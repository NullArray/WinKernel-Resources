#include <ntddk.h>
#include <windef.h>
#include "pe.h"

#pragma section(".code", execute,read,write)
#pragma comment(linker,"/MERGE:.data=.code")
#pragma comment(linker,"/SECTION:.code,ERW")
#pragma code_seg(push, ".code")


#define IA32_SYSENTER_EIP 0x176

typedef struct _MSR
{
	DWORD loValue;
	DWORD hiValue;
} MSR, *PMSR;


void getMSR(DWORD regAddress, PMSR msr);
BOOLEAN GetLibAddressViaShellCode(DWORD* routineAddress, DWORD hash);
BOOLEAN walk_export_list(DWORD dll_base, DWORD *function_ptr, DWORD function_hash);
DWORD getHashA(char *str);
DWORD getHashW(WCHAR *str);


typedef NTSTATUS( *ZwReadFile_)(
	HANDLE,
	HANDLE,
	PIO_APC_ROUTINE,
	PVOID ,
	PIO_STATUS_BLOCK,
	PVOID,
	ULONG,
	PLARGE_INTEGER,
	PULONG
);

NTSTATUS
DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	ZwReadFile_ zwreadfile;

	DWORD ZwReadFile_hash = 0x0003AB62;

	GetLibAddressViaShellCode((DWORD*)&zwreadfile, ZwReadFile_hash);

	return STATUS_SUCCESS;
}


void getMSR(DWORD regAddress, PMSR msr)
{
	DWORD loValue;
	DWORD hiValue;

	__asm
	{
		MOV ecx, regAddress; // set on ecx register to read
		rdmsr;
		MOV hiValue, edx;
		MOV loValue, eax;
	}

	msr->loValue = loValue;
	msr->hiValue = hiValue;
}


BOOLEAN GetLibAddressViaShellCode(DWORD* routineAddress, DWORD hash)
{
	MSR msr;
	BYTE *bytePtr;
	DWORD baseAddress;

	getMSR(IA32_SYSENTER_EIP, &msr);
	KdPrint(("[GetLibAddressViaShellCode]: address nt!KiFastCallEntry() = 0x%08X\n", msr.loValue));
	bytePtr = (BYTE*)(msr.loValue & 0xFFFFF000); // get only high part of address

	//WARNING: compare more than two bytes
	while
	(
		(bytePtr[0] != 'M') ||
		(bytePtr[1] != 'Z') ||
		(bytePtr[2] != 0x90)
	)
	{
		bytePtr = (BYTE*)((DWORD)bytePtr - 0x1000);
	}

	baseAddress = (DWORD)bytePtr;
	KdPrint(("[GetLibAddressViaShellCode]: address of nt module = 0x%08X\n", baseAddress));
	return (walk_export_list(baseAddress, routineAddress, hash));	
}


BOOLEAN walk_export_list(DWORD dll_base, DWORD *function_ptr, DWORD function_hash)
{
	IMAGE_DOS_HEADER* image_dos_header = (IMAGE_DOS_HEADER*)dll_base;
	IMAGE_NT_HEADERS* image_nt_headers = (IMAGE_NT_HEADERS*)(dll_base + image_dos_header->e_lfanew);
	IMAGE_DATA_DIRECTORY* image_data_directory = &(image_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);

	DWORD descriptor_start_rva = image_data_directory->VirtualAddress;

	IMAGE_EXPORT_DIRECTORY* export_directory = (PIMAGE_EXPORT_DIRECTORY)(dll_base + descriptor_start_rva);

	DWORD* address_of_names = (DWORD*)(dll_base + export_directory->AddressOfNames);
	DWORD* address_of_functions = (DWORD*)(dll_base + export_directory->AddressOfFunctions);
	WORD* address_of_ordinals = (WORD*)(dll_base + export_directory->AddressOfNameOrdinals);

	SIZE_T index;

	for (index = 0; index < export_directory->NumberOfNames; index++)
	{
		char* name;
		DWORD name_rva;
		WORD ordinal;

		name_rva = address_of_names[index];

		if (name_rva == 0)
			continue;

		name = (char *)(dll_base + name_rva);
	
		if (getHashA(name) == function_hash)
		{
			ordinal = address_of_ordinals[index];
			(*function_ptr) = (dll_base + address_of_functions[ordinal]);
			return (TRUE);
		}
	}

	return (FALSE);
}


DWORD getHashA(char *str)
{
	DWORD hash;
	hash = 0;

	while (*str != '\0')
	{
		hash = hash + (*(unsigned char*)str | 0x60);
		hash = hash << 1;
		str = str + 1;
	}
	return (hash);
}


DWORD getHashW(WCHAR* str)
{
	DWORD hash;
	hash = 0;

	while (*str != '\0')
	{
		hash = hash + *str;
		str = str + 1;
	}

	return (hash);
}

#pragma code_seg(pop)