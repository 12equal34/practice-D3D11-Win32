#include <Windows.h>
#include <ShObjIdl.h>
#include <string>
#include "Window.h"

// - Initializing the COM Library -
// Any Windows program that uses COM must initialize the COM library by calling
// the CoInitializeEx function. Each thread that uses a COM interface must make
// a separate call to the CoInitializeEx function.
// HRESULT CoInitializeEx(LPVOID pvReserved, DWORD dwCoInit);
// The frist param is reserved and must be NULL.
// The second param specifies the threading model that your program will use.
// COM supports two different threading models: apartmentthreaded,
// multithreaded.
// If You will access each COM object from a single thread and
// the thread will have a message loop, then use apartmentthreaded model.
// If not, then use multithreaded model.
// In addition to the flags, set the COINIT_DISABLE_OLE1DDE flag in the dwCoInit
// param for avoiding some overhead associated with Object Linking and Embedding
// (OLE) 1.0, an obsolete technology.
// For every successful call to CoInitializeEx, you must call CoUninitialize
// before the thread exits.

// - Error Codes in COM -
// An HRESULT is a 32-bit integer. The high-order bit of the HRESULT means
// success(0) or failure(1)
// To check whether a COM method succeeds, examine the high-order bit of the
// returned HRESULT. Use two macros : SUCCEEDED, FAILED

// - Creating an Object in COM -
// To use a COM interface, your program first creates an instance of an object
// that implements that interface.
// In general, there are two ways to create a COM object:
// The module that implements the object might provide a function specifically
// designed to create instances of that object. Alternatively, COM provides a
// generic creation function named CoCreateInstance.
//
// COM uses pass-by-address, not pass-by-reference.
// Ex) HRESULT CreateShape(IDrawable** ppShape);
//     IDrawable *pShape;
//     HRESULT hr = CreateShape(&pShape);
//
// CoCreateInstance(5 params): a generic way to create objects
// A GUID (globally unique identifier, a 128-bit number) identifies an object or
// an interface.
// GUIDs are sometimes called UUIDs (universally unique identifiers).
// Which object to create: ex) CLSID_Shape identifies the Shape object.
// Which interface to get from the object:
//                         ex) IID_IDrawable identifies the IDrawable interface.
// EX)
//      IDrawable *pShape;
//      hr = CoCreateInstance(CLSID_Shape, NULL, CLSCTX_INPROC_SERVER,
//                            IID_IDrawable, reinterpret_cast<void**>(&pShape));
//
//      if (SUCCEEDED(hr))
//      {
//          // Use the Shape object.
//      } else
//      {
//          // An error occurred.
//      }

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
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr)) {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // display the file name to the user.
                    if (SUCCEEDED(hr)) {
                        MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }

    return 0;
}