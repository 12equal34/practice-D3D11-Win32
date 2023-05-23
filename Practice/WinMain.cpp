#include "App.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    try {
        App app;
        return app.Run();
    } catch (const BaseException& ex) {
        MessageBoxA(nullptr, ex.what(), ex.GetType(),
                    MB_OK | MB_ICONEXCLAMATION);
    } catch (const std::exception& ex) {
        MessageBoxA(nullptr, ex.what(), "Standard Exception",
                    MB_OK | MB_ICONEXCLAMATION);
    } catch (...) {
        MessageBoxA(nullptr, "Not defined exception", "Unknown Exception",
                    MB_OK | MB_ICONEXCLAMATION);
    }
    return -1;
}