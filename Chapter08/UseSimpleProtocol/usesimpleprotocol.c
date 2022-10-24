#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Protocol/SimpleFileSystem.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include "Protocol/trysimpleprotocol.h"

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_SIMPLE_PROTOCOL *simpleProtocol;
    gBS->LocateProtocol(&gEfiSimpleProtocolGUID, NULL, (void**)&simpleProtocol);
    INT64 totalLen = 0;
    simpleProtocol->SimpleFunc(simpleProtocol, L"Thank you", &totalLen);
    Print(L"Total Len: %d\n", totalLen);
    return EFI_SUCCESS;
}