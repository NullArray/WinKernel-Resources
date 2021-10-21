#include <ntddk.h>

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath);
VOID Unload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS OnStubDispatch(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

/***
*	Functions to handle different IRPs
*/
NTSTATUS MyOpen(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS MyClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS MyRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS MyWrite(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS MyIOControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
// Driver entry point
DRIVER_INITIALIZE DriverEntry;

/*
*	Possible functions in case we want to
*	implement functionalities of open, reading,
*	writing or closing of drivers
*/

NTSTATUS MyOpen(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(Irp);

	DbgPrint("%s\n", "Called IRP for Open (example CreateFile of driver)");
	
	return STATUS_SUCCESS; // always return a status!!!
}

NTSTATUS MyClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(Irp);

	DbgPrint("%s\n", "Called IRP for close (example CloseHandle of driver)");
	
	return STATUS_SUCCESS;
}

NTSTATUS MyRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(Irp);

	DbgPrint("%s\n", "Called IRP for read (example ReadFile of driver)");

	return STATUS_SUCCESS;
}

NTSTATUS MyWrite(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(Irp);

	DbgPrint("%s\n", "Called IRP for write (example WriteFile of driver");

	return STATUS_SUCCESS;

}

NTSTATUS MyIOControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	PIO_STACK_LOCATION	IrpSp;
	ULONG				FunctionCode = 0;
	NTSTATUS			statusF = STATUS_SUCCESS;

	UNREFERENCED_PARAMETER(DeviceObject);

	// get stack of IRP to get the parameters
	IrpSp = IoGetCurrentIrpStackLocation(Irp);

	/*
	*	From the parameters of the stack, we get the one of DeviceIoControl
	*	as it will be the function we are going to manage from MyIOControl. 
	*	Then from these we will take the IoControlCode
	*/
	FunctionCode = IrpSp->Parameters.DeviceIoControl.IoControlCode;

	switch (FunctionCode)
	{
		/*
		* Depending on function code from IoControl 
		* do different things
		*/
	case 0xDEADBEEF:
		DbgPrint("%s\n", "DEADBEEF");
		statusF = STATUS_SUCCESS;
		break;
	case 0xCAFECAFE:
		DbgPrint("%s\n", "CAFECAFE");
		statusF = STATUS_SUCCESS;
		break;
	default:
		statusF = STATUS_NOT_IMPLEMENTED;
		break;
	}

	Irp->IoStatus.Status = statusF;
	
	// set the IoCompleteRequest
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return statusF;
}

// global variables
const WCHAR deviceNameBuffer[] = L"\\Device\\Example"; // name to access from user space to the driver
const WCHAR deviceLinkBuffer[] = L"\\DosDevices\\Alias"; // alias of the driver

PDEVICE_OBJECT g_rootkitdevice; // global pointer to our device object

/*
*	Generic function for IRP 
*/
NTSTATUS OnStubDispatch(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	Irp->IoStatus.Status = STATUS_SUCCESS;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
	DbgPrint("%s\n", "Driver unloaded.");
	return;
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	NTSTATUS		ntStatus;
	UNICODE_STRING	deviceNameUnicodeString;
	UNICODE_STRING	deviceLinkUnicodeString;
	size_t			i;

	UNREFERENCED_PARAMETER(RegistryPath);

	// get name and symbolic link in unicode
	RtlInitUnicodeString(&deviceNameUnicodeString, deviceNameBuffer);
	RtlInitUnicodeString(&deviceLinkUnicodeString, deviceLinkBuffer);

	// create the device
	ntStatus = IoCreateDevice(DriverObject,
		0,							// for driver extention
		&deviceNameUnicodeString,	// device name
		0x00001234,					// device type
		0,							// characteristics
		TRUE,						// Exclusive
		&g_rootkitdevice			// pointer to device object
	);

	if (NT_SUCCESS(ntStatus))
	{
		// set symbolic name
		ntStatus = IoCreateSymbolicLink(&deviceLinkUnicodeString, &deviceNameUnicodeString);
	}

	// set unload function
	DriverObject->DriverUnload = Unload;

	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		// set Major functions
		// set all of them to OnStubDispatch and then
		// we will set the others
		DriverObject->MajorFunction[i] = OnStubDispatch;
	}

	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]	= MyIOControl;
	DriverObject->MajorFunction[IRP_MJ_READ]			= MyRead;
	DriverObject->MajorFunction[IRP_MJ_WRITE]			= MyWrite;
	DriverObject->MajorFunction[IRP_MJ_CREATE]			= MyOpen;
	DriverObject->MajorFunction[IRP_MJ_CLOSE]			= MyClose;

	return STATUS_SUCCESS;
}