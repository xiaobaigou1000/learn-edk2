#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_EVENT events[2] = {0};
    events[0] = gST->ConIn->WaitForKey;
    gBS->CreateEvent(EVT_TIMER, TPL_CALLBACK, NULL, NULL, &events[1]);
    gBS->SetTimer(events[1], TimerPeriodic, 10 * 1000 * 1000);
    UINTN eventIndex = 0;
    EFI_STATUS result = gBS->WaitForEvent(2, events, &eventIndex);
    if (result != EFI_SUCCESS)
    {
        Print(L"Error");
        return EFI_UNSUPPORTED;
    }
    // return EFI_SUCCESS;
    if (eventIndex == 0)
    {
        EFI_INPUT_KEY efiInputKey;
        gST->ConIn->ReadKeyStroke(gST->ConIn, &efiInputKey);
        CHAR16 unicodeKeyCode[2] = {0};
        unicodeKeyCode[0] = efiInputKey.UnicodeChar;
        Print(L"Scan Code: %u, Unicode Char: %s\n", efiInputKey.ScanCode, unicodeKeyCode);
    }
    else if (eventIndex == 1)
    {
        Print(L"No input in 1 sec\n");
    }

    return EFI_SUCCESS;
}
