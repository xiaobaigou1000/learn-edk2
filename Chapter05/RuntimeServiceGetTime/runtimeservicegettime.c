#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_TIME time;
    EFI_STATUS status;
    status = gRT->GetTime(&time, NULL);
    ASSERT_EFI_ERROR(status);
    Print(L"Get boot time:\n");
    Print(L"%d.%d.%d %d:%d:%d \n", time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second);
    return EFI_SUCCESS;
}
