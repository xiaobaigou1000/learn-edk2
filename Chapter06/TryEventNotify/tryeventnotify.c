#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

VOID EFIAPI MyEventNotify(IN EFI_EVENT event, IN VOID *context)
{
    static UINTN count = 0;
    Print(L"My event notify times: %d\n", count);
    ++count;
    if (count > 5)
    {
        gBS->SignalEvent(event);
    }
}

EFI_STATUS EFIAPI TestNotify()
{
    EFI_EVENT event = NULL;
    gBS->CreateEvent(EVT_NOTIFY_WAIT, TPL_NOTIFY, (EFI_EVENT_NOTIFY)MyEventNotify, NULL, &event);
    UINTN index = 0;
    gBS->WaitForEvent(1, &event, &index);
    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    TestNotify();
    return EFI_SUCCESS;
}
