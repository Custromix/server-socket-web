#include "framework.h"

SClient::SClient(SOCKET clientSocket, HWND window)
{
	clientSocket_ = clientSocket;

	WSAAsyncSelect(clientSocket, window, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);
}

SClient::~SClient()
{
}

void SClient::SetSockAddr(sockaddr_in sockAddr)
{
	clientAddr_ = sockAddr;
	clientAddrSize_ = sizeof(clientAddr_);
}