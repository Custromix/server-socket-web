#ifdef _DEBUG
#include <crtdbg.h>
#endif

#include "framework.h"

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
    _CrtMemState memStateInit;
    _CrtMemCheckpoint(&memStateInit);
#endif

    HWND hwnd;
    MSG msg;
    WNDCLASS wc;

    wc.style = 0;
    wc.lpfnWndProc = WinMessHandler::MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hinstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(1 + COLOR_BTNFACE);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"MaWinClass";

    if (!RegisterClass(&wc)) return FALSE;

    hwnd = CreateWindowA("MaWinClass", "Titre", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, hinstance, NULL);
    if (!hwnd) return FALSE;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);


    // Allouer une console
    AllocConsole();

    // Rediriger la sortie standard vers la console
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

    //std::cout << "Hello, world!" << std::endl;

    Server* server = new Server(28489, 2, hwnd);

    try {

        server->Start();

        server->Listen();


    }
    catch (const std::exception& e) {

        server->Stop();
        std::cout << "Exception : " << e.what() << std::endl;
    }


    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    // Fermer la console
    FreeConsole();

#ifdef _DEBUG
    _CrtMemState memStateEnd, memStateDiff;
    _CrtMemCheckpoint(&memStateEnd);
    if (_CrtMemDifference(&memStateDiff, &memStateInit, &memStateEnd))
    {
        MessageBoxA(NULL, "MEMORY LEAKS", "DISCLAIMER", 0);
    }
#endif 

    return msg.wParam;
}
/****************************************************************************/