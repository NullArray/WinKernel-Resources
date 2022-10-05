#include <ntddk.h>
#include <windef.h>

#include "../Kernel_shellcode/pe.h"

ULONG ZwGetFileSize(HANDLE hFile);
BOOLEAN getShCodeParameters(BYTE* buffer, DWORD* offset, DWORD* size);
BOOLEAN checkDosHeader(BYTE* buffer, DWORD* ntHeaderAddress);
BOOLEAN digestNTHeaders(DWORD ntHeaderAddress, DWORD* nSections, DWORD* sectionTableAddress);

#define FILE_PATH L"\\DosDevices\\C:\\ShCode.bin"
#define MEM_TAG 'BOB'
#define FILE_TAG '0000'
#define SHELL_TAG '0001'

#define EXEC_CODE(address) __asm MOV edx, address __asm call edx

extern "C"
NTSTATUS
DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);


	UNICODE_STRING fileName;
	OBJECT_ATTRIBUTES fileAttributes;
	NTSTATUS ntStatus;
	HANDLE fileHandle;
	IO_STATUS_BLOCK statusBlock;
	ULONG fileSize;

	BYTE *fileBuffer, *shellBuffer;
	DWORD shellOffset, shellSize;
	SIZE_T i;

	fileName = RTL_CONSTANT_STRING(FILE_PATH);
	
	InitializeObjectAttributes
	(
		&fileAttributes,
		&fileName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL
	);

	// open the file, copy its bytes into RAM and close the file
	ntStatus = ZwOpenFile
	(
		&fileHandle,
		SYNCHRONIZE | GENERIC_READ,
		&fileAttributes,
		&statusBlock,
		0,
		FILE_SYNCHRONOUS_IO_ALERT
	);
	if (!NT_SUCCESS(ntStatus))
	{
		KdPrint(("[DriverEntry]: Error opening file\n"));
		return ntStatus;
	}

	fileSize = ZwGetFileSize(fileHandle);

	fileBuffer = (BYTE*)ExAllocatePoolWithTag
	(
		NonPagedPool,
		fileSize,
		FILE_TAG
	);

	if (fileBuffer == nullptr)
	{
		KdPrint(("[DriverEntry]: Error allocating memory for file\n"));
		ExFreePool(fileBuffer);
		return STATUS_MEMORY_NOT_ALLOCATED;
	}

	ntStatus = ZwReadFile
	(
		fileHandle,
		NULL,
		NULL,
		NULL,
		&statusBlock,
		(PVOID)fileBuffer,
		fileSize,
		NULL,
		NULL
	);
	if (!NT_SUCCESS(ntStatus))
	{
		KdPrint(("[DriverEntry]: Error reading file\n"));
		ExFreePool(fileBuffer);
		return ntStatus;
	}

	ntStatus = ZwClose(fileHandle);
	if (!NT_SUCCESS(ntStatus))
	{
		KdPrint(("[DriverEntry]: Error closing file\n"));
		ExFreePool(fileBuffer);
		return ntStatus;
	}


	getShCodeParameters(fileBuffer, &shellOffset, &shellSize);

	shellBuffer = (BYTE*)ExAllocatePoolWithTag
	(
		NonPagedPool,
		shellSize,
		SHELL_TAG
	);

	if (shellBuffer == nullptr)
	{
		KdPrint(("[DriverEntry]: Error allocating memory for shellcode\n"));
		ExFreePool(fileBuffer);
		return STATUS_MEMORY_NOT_ALLOCATED;
	}

	for (i = 0; i < shellSize; i++)
	{
		shellBuffer[i] = fileBuffer[shellOffset + i];
	}

	ExFreePool(fileBuffer);


	KdPrint(("[DriverEntry]: executing buffer\n"));
	EXEC_CODE(shellBuffer);
	KdPrint(("[DriverEntry]: done executing buffer\n"));

	ExFreePool(shellBuffer);

	return STATUS_SUCCESS;
}


ULONG ZwGetFileSize(HANDLE hFile) {
	ULONG nRet = 0;
	IO_STATUS_BLOCK StatusBlock;
	FILE_STANDARD_INFORMATION Info;
	if (NT_SUCCESS(ZwQueryInformationFile(hFile,
		&StatusBlock,
		&Info,
		sizeof(Info),
		FileStandardInformation)))
	{
		return Info.EndOfFile.LowPart;
	}

	return nRet;
}

BOOLEAN getShCodeParameters(BYTE* buffer, DWORD* offset, DWORD* size)
{
	BOOL ok;
	DWORD dosHeaderAddress;
	DWORD ntHeaderAddress;
	DWORD nSections;
	DWORD secionTableAddress;
	DWORD index;
	IMAGE_SECTION_HEADER* sectionHeader;

	dosHeaderAddress = (DWORD)buffer;
	ok = checkDosHeader(buffer, &ntHeaderAddress);

	if (!ok)
	{
		KdPrint(("[getShCodeParameters]: Header check failed\n"));
		return (FALSE);
	}

	ok = digestNTHeaders(ntHeaderAddress, &nSections, &secionTableAddress);

	if (!ok)
	{
		KdPrint(("[getShCodeParameters]: NT Header check failed\n"));
		return (FALSE);
	}

	// iterate through section headers

	sectionHeader = (IMAGE_SECTION_HEADER*)secionTableAddress;
	for (index = 0; index < nSections; index++)
	{
		KdPrint(("[getShCodeParameters]: section %d\n", index));

		if
			(
				sectionHeader[index].Name[0] == '.' &&
				sectionHeader[index].Name[1] == 'c' &&
				sectionHeader[index].Name[2] == 'o' &&
				sectionHeader[index].Name[3] == 'd' &&
				sectionHeader[index].Name[4] == 'e'
				)
		{
			KdPrint(("[getShCodeParameters]: found .code\n"));
			*offset = sectionHeader[index].PointerToRawData;
			*size = sectionHeader[index].SizeOfRawData;
			KdPrint(("[getShCodeParameters]: call success\n"));
			return (TRUE);
		}
	}

	KdPrint(("[getShCodeParameters]: Couldn't find .code section\n"));
	return (FALSE);
}


BOOLEAN checkDosHeader(BYTE* buffer, DWORD* ntHeaderAddress)
{
	IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*)buffer;

	if (dos_header->e_magic != IMAGE_DOS_SIGNATURE)
	{
		KdPrint(("[checkDosHeader]: Error magic value not correct\n"));
		*ntHeaderAddress = 0;
		return (FALSE);
	}

	*ntHeaderAddress = (DWORD)buffer + dos_header->e_lfanew;
	return (TRUE);
}


BOOLEAN digestNTHeaders(DWORD ntHeaderAddress, DWORD* nSections, DWORD* sectionTableAddress)
{
	IMAGE_NT_HEADERS* nt_headers = (IMAGE_NT_HEADERS*)ntHeaderAddress;

	if (nt_headers->Signature != IMAGE_NT_SIGNATURE)
	{
		KdPrint(("[digestNTHeaders]: NT Signature not correct\n"));
		*nSections = 0;
		*sectionTableAddress = 0;
		return (FALSE);
	}

	*nSections = nt_headers->FileHeader.NumberOfSections;
	*sectionTableAddress = (ntHeaderAddress + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + nt_headers->FileHeader.SizeOfOptionalHeader);
	return (TRUE);
}
