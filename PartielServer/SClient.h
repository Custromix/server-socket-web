#pragma once
class SClient
{

private:
    SOCKET clientSocket_;
    sockaddr_in clientAddr_;
    int clientAddrSize_;


public:

    SClient(SOCKET clientSocket, HWND window);

    ~SClient();

    SOCKET* GetSocket()                                                         { return &clientSocket_; }
    sockaddr_in* GetSockAddr()                                                  { return &clientAddr_; }
    int* GetSockAddrSize()                                                      { return &clientAddrSize_; }

    void SetSocket(SOCKET sock)                                                 { clientSocket_ = sock; }
    void SetSockAddr(sockaddr_in sockAddr);

};

