#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_EVENT event;
    Print(L"Test EVT_TIMER\n");
    EFI_STATUS status = gBS->CreateEvent(EVT_TIMER, TPL_CALLBACK, NULL, NULL, &event);
    if (status != EFI_SUCCESS)
    {
        Print(L"create event failed.\n");
        return EFI_UNSUPPORTED;
    }
    status = gBS->SetTimer(event, TimerPeriodic, 10 * 1000 * 1000);
    if (status != EFI_SUCCESS)
    {
        Print(L"set timer failed.\n");
        return EFI_UNSUPPORTED;
    }
    while (TRUE)
    {
        UINTN index = 0;
        status = gBS->WaitForEvent(1, &event, &index);
        Print(L"timer event triggered.\n");
        if (status != EFI_SUCCESS)
        {
            Print(L"wait for event failed.\n");
            return EFI_UNSUPPORTED;
        }
    }
    gBS->CloseEvent(event);

    return EFI_SUCCESS;
}