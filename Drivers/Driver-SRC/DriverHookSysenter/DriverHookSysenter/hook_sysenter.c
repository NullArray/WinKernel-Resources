#include <ntddk.h>
#include <stdio.h>
#include <stdlib.h>

/*
*	Hook based on the instruction SYSENTER instead of INT 2E
*	this is the instruction which replaced int 2e for a fast call
*	method. Ntdll loads in EAX register the syscall to use, and
*	saves in EDX a pointer to the stack with the parameters.
*	The call to SYSENTER gives the control to the address specified
*	in one of the model specific registers (MSRs). Specifically
*	to the one called IA32_SYSENTER_EIP. It is possible to read
*	or write from/to that register from Ring 0.
*/

// specific to the register
#define IA32_SYSENTER_EIP	0x176
// structure representing the MSR
typedef struct _MSR
{
	ULONG loValue;
	ULONG hiValue;
} MSR, *PMSR;

// Original value of ntoskrnl!KiFastCallEntry
MSR d_origKiFastCallEntry;

/*
*	Function declared as Hook,
*	it is declared as naked to
*	avoid any instructions more
*	than those inside of the function.
*
*	When this function is called we will
*	have in EAX the syscall, and in EDX
*	the pointer to the parameters in
*	the stack.
*/
__declspec(naked) MyKiFastCallEntry()
{
	__asm
	{
		jmp[d_origKiFastCallEntry.loValue];
	}
}

// functions that we will use
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath);
VOID Unload(IN PDRIVER_OBJECT DriverObject);
VOID SetMSRValue(ULONG regAddress, MSR msr);
VOID GetMSRValue(ULONG regAddress, PMSR msr);

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	MSR	own_msr;

	UNREFERENCED_PARAMETER(RegistryPath);

	DriverObject->DriverUnload = Unload;

	own_msr.hiValue = (ULONG)0;
	own_msr.loValue = (ULONG)MyKiFastCallEntry;

	GetMSRValue(IA32_SYSENTER_EIP, &d_origKiFastCallEntry);
	SetMSRValue(IA32_SYSENTER_EIP, own_msr);
	
	return STATUS_SUCCESS;
}

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);

	SetMSRValue(IA32_SYSENTER_EIP, d_origKiFastCallEntry);
}

VOID SetMSRValue(ULONG regAddress, MSR msr)
{
	ULONG loValue = msr.loValue;
	ULONG hiValue = msr.hiValue;

	__asm
	{
		mov ecx, regAddress;		// set in ecx the entry of the MSR
		mov edx, hiValue;			// set the high value
		mov eax, loValue;			// set the low value
		wrmsr;						// write the new MSR
	}
}

VOID GetMSRValue(ULONG regAddress, PMSR msr)
{
	ULONG loValue;
	ULONG hiValue;

	__asm
	{
		mov ecx, regAddress;		// set in ecx the entry of the MSR
		rdmsr;						// read the MSR
		mov hiValue, edx;			// get the high value
		mov loValue, eax;			// get the low value
	}

	msr->loValue = loValue;
	msr->hiValue = hiValue;
}

