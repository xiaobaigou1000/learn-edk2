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
    EFI_SIMPLE_POINTER_PROTOCOL *mouseProtocol;
    gBS->LocateProtocol(&gEfiSimplePointerProtocolGuid, NULL, (VOID**)&mouseProtocol);
    mouseProtocol->Reset(mouseProtocol, TRUE);
    events[0] = mouseProtocol->WaitForInput;
    events[1] = gST->ConIn->WaitForKey;
    while (TRUE)
    {
        UINTN index = 0;
        gBS->WaitForEvent(2, events, &index);
        if (index == 0)
        {
            EFI_SIMPLE_POINTER_STATE state;
            mouseProtocol->GetState(mouseProtocol, &state);
            Print(L"relative x: %d, relative y: %d, relative z: %d, left mouse button: %d, right mouse button: %d\n",
                  state.RelativeMovementX,
                  state.RelativeMovementY,
                  state.RelativeMovementZ,
                  state.LeftButton,
                  state.RightButton);
        }
        else
        {
            EFI_INPUT_KEY key;
            gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
            if (key.ScanCode == 23)
            {
                break;
            }
        }
    }

    return EFI_SUCCESS;
}
