#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SimplePointer.h>

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_EVENT events[2] = {NULL};
    events[1] = gST->ConIn->WaitForKey;
    gST->ConOut->EnableCursor(gST->ConOut, TRUE);
    EFI_SIMPLE_POINTER_PROTOCOL mouseProtocol;
    gBS->LocateProtocol(&gEfiSimplePointerProtocolGuid, NULL, &mouseProtocol);
    
    return EFI_SUCCESS;
}
