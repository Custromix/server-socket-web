#include "framework.h"



LRESULT CALLBACK WinMessHandler::MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        return 0;

    case WM_PAINT:
        return 0;

    case WM_SOCKET:
    {
        if (WSAGETSELECTERROR(lParam))
        {
            closesocket((SOCKET)wParam);
            break;
        }

        Server* pServer = (Server*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        
        switch (WSAGETSELECTEVENT(lParam))
        {
            case FD_ACCEPT: 
            {
                if (pServer->GetClients()->size() < pServer->GetSlot())
                {

                    if (pServer->Connect(wParam)) {
                        printf("Client connecté ! \n");
                    }
                }

                break;
            }

            case FD_READ: 
            {
                SOCKET emitSock = (SOCKET)wParam;
                SClient* emitter = new SClient(emitSock, hwnd);

                pServer->ReceiveFromClient(emitter);

                break;
            }

            case FD_WRITE:
            {
                if (!pServer->GetSendedMessage()->IsEmpty())
                {
                    SMessage message = pServer->GetSendedMessage()->DequeueMessage();
                }
                break;
            }

            case FD_CLOSE: 
            {
                SOCKET emitSock = (SOCKET)wParam;
                SClient* emitter = new SClient(emitSock, hwnd);

                closesocket(emitSock);
                delete emitter;
                printf("Le client s'est déconnecté\n");
                break;
            }
        }

        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}