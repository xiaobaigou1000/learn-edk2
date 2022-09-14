#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DiskIo.h>

VOID EFIAPI WalthroughDevicePath(EFI_DEVICE_PATH_PROTOCOL *devicePath)
{
    EFI_DEVICE_PATH_PROTOCOL *pDevicePath = devicePath;
    Print(L"    ");
    while (!IsDevicePathEnd(pDevicePath))
    {
        Print(L"(%d %d)/", pDevicePath->Type, pDevicePath->SubType);
        pDevicePath = NextDevicePathNode(pDevicePath);
    }
    Print(L"\n");
}

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *deviceToTextProtocol;
    EFI_STATUS status = gBS->LocateProtocol(&gEfiDevicePathToTextProtocolGuid, NULL, (void **)&deviceToTextProtocol);
    if (EFI_ERROR(status))
    {
        DebugPrint(DEBUG_ERROR, "locate protocol failed. exit!\n");
        return EFI_UNSUPPORTED;
    }
    UINTN numHandles;
    EFI_HANDLE *controllerHandle;
    status = gBS->LocateHandleBuffer(ByProtocol, &gEfiDiskIoProtocolGuid, NULL, &numHandles, &controllerHandle);
    if (EFI_ERROR(status))
    {
        DebugPrint(DEBUG_ERROR, "locate handle buffer failed.\n");
        return status;
    }

    Print(L"Num Handles: %d\n", numHandles);

    for (UINTN i = 0; i < numHandles; i++)
    {
        EFI_DEVICE_PATH_PROTOCOL *diskDevicePath;
        status = gBS->OpenProtocol(controllerHandle[i], &gEfiDevicePathProtocolGuid, (void **)&diskDevicePath, gImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
        if (EFI_ERROR(status))
        {
            continue;
        }

        CHAR16 *textDevicePath = deviceToTextProtocol->ConvertDevicePathToText(diskDevicePath, TRUE, TRUE);
        Print(L"%s\n", textDevicePath);
        if (textDevicePath)
        {
            gBS->FreePool(textDevicePath);
        }
        WalthroughDevicePath(diskDevicePath);
    }

    return EFI_SUCCESS;
}
