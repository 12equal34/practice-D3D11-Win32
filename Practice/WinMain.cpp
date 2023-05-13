#include <Windows.h>
#include <ShObjIdl.h>
#include <string>
#include "Window.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    HRESULT hr =
        CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr)) {
        IFileOpenDialog* pFileOpen;

        // create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                              IID_IFileOpenDialog,
                              reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr)) {
            // show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // get the file name from the dialog box
            if (SUCCEEDED(hr)) {
                // Asking an Object for an Interface
                IFileDialogCustomize* pCustom;
                hr = pFileOpen->QueryInterface(
                    IID_IFileDialogCustomize,
                    reinterpret_cast<void**>(&pCustom));
                if (SUCCEEDED(hr)) {
                    // use the interface.

                    pCustom->Release();
                } else {
                    // handle the error.
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }

    return 0;
}