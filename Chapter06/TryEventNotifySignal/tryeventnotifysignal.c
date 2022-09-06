#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

VOID EFIAPI timerNotifyFunc(EFI_EVENT event, VOID *context)
{
    static UINTN count = 0;
    count++;
    Print(L"Timer triggered %d times.\n", count);
    EFI_TIME time;
    gRT->GetTime(&time,NULL);
    Print(L"Current System Time: %d.%d.%d %d:%d:%d \n", time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second);
}

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_EVENT event;
    Print(L"Test EVT_NOTIFY_SIGNAL\n");
    EFI_STATUS status = gBS->CreateEvent(EVT_TIMER | EVT_NOTIFY_SIGNAL, TPL_CALLBACK, timerNotifyFunc, NULL, &event);
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
    UINTN index = 0;
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &index);
    gBS->CloseEvent(event);
    return EFI_SUCCESS;
}
