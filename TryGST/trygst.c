#include<Uefi.h>
#include<Library/UefiApplicationEntryPoint.h>
#include<Library/UefiLib.h>
#include<Library/UefiBootServicesTableLib.h>

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  while(TRUE){
    UINTN index;
    SystemTable->BootServices->WaitForEvent(1,&SystemTable->ConIn->WaitForKey,&index);
    EFI_INPUT_KEY key;
    EFI_STATUS status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn,&key);
    if(EFI_ERROR(status)){
      Print(L"Error reading key\n");
    }
    CHAR16 keyPressed[3]={0};
    keyPressed[1]='\n';
    keyPressed[0]=key.UnicodeChar;
    Print(keyPressed);
    if(key.ScanCode == 0x0017){
      break;
    }
  }
  return EFI_SUCCESS;
}
