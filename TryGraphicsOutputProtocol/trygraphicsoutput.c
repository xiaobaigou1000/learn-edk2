#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Protocol/SimpleFileSystem.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Protocol/GraphicsOutput.h>

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL *graphicsOutput = NULL;
    gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (void *)&graphicsOutput);
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *graphicsModes;
    UINTN sizeOfGraphicsModes = 0;
    graphicsOutput->QueryMode(graphicsOutput, 0, &sizeOfGraphicsModes, &graphicsModes);
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel;
    pixel.Red = 0xFF;
    pixel.Blue = 0x00;
    pixel.Green = 0x00;
    pixel.Reserved = 0x00;
    graphicsOutput->SetMode(graphicsOutput, 0);
    graphicsOutput->Blt(graphicsOutput, &pixel, EfiBltVideoFill, 0, 0, 0, 0, graphicsModes->HorizontalResolution, graphicsModes->VerticalResolution, 0);
    UINTN eventIndex = 0;
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &eventIndex);
    return EFI_SUCCESS;
}