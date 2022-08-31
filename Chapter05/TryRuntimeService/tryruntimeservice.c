#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_TIME time;
    EFI_STATUS status = gRT->GetTime(&time, NULL);
    ASSERT(status == EFI_SUCCESS);
    Print(L"Get boot time:\n");
    Print(L"%d.%d.%d %d:%d:%d \n", time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second);
    return EFI_SUCCESS;
}
