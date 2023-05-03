#include "framework.h"

Server::Server(int port, int slot, HWND hwnd)
{
    PORT = port;
    slot_ = slot;
    WINDOW = hwnd;
    queueReceive = new QueueMessageManager();
    queueWrite = new QueueMessageManager();

}

Server::~Server()
{
}



void Server::Start()
{
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        throw std::runtime_error("Erreur lors de l'initialisation de Winsock : " + WSAGetLastError());
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        throw std::runtime_error("Erreur de création du socket : " + WSAGetLastError());
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        throw std::runtime_error("Erreur lors de la liaison du socket : " + WSAGetLastError());
    }

    WSAAsyncSelect(serverSocket, WINDOW, WM_SOCKET, FD_ACCEPT | FD_CLOSE);

    SetWindowLongPtr(WINDOW, GWLP_USERDATA, (LONG_PTR)this);
}

void Server::Listen()
{
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
        throw std::runtime_error("Erreur lors de la mise en mode écoute du socket : " + WSAGetLastError());
    else
        std::cout << "Le serveur est à l'écoute sur le port : " << serverAddr.sin_port << std::endl;
}

void Server::Stop()
{
    closesocket(serverSocket);
    WSACleanup();
}

CResult<int>* Server::IsExist(SClient* client)
{
    CResult<int>* result = new CResult<int>(false);

    if (client == nullptr) {
        throw std::invalid_argument("Client pointer is null.");
    }

    for (int i = 0; i < connectedClient.size(); i++) {
        if (connectedClient[i]->GetSockAddr() == client->GetSockAddr()) {
            result->SetSatue(true);
            result->SetValue(i);
            return result;
        }
    }

    return result;
}

bool Server::Join(SClient* joiningClient)
{
    if (!IsExist(joiningClient)->GetStatus())
    {
        connectedClient.push_back(joiningClient);
        return true;
    }
    else
        return false;

}

bool Server::Connect(WPARAM wParam)
{
    SOCKET clientSocket;
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    int numberOfClient = 0;

    if ((clientSocket = accept(wParam, (SOCKADDR*)&clientAddr, &clientAddrSize)) != INVALID_SOCKET) {
        SClient* client = new SClient(clientSocket, WINDOW);
        client->SetSockAddr(clientAddr);
        if (Join(client))
            numberOfClient++; 
    }

    return true;
}


bool Server::Disconnect(WPARAM wParam)
{
    SOCKET clientSocket;
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    int numberOfClient = 0;

    if ((clientSocket = accept(wParam, (SOCKADDR*)&clientAddr, &clientAddrSize)) != INVALID_SOCKET) {
        SClient* player = new SClient(clientSocket, WINDOW);
        player->SetSockAddr(clientAddr);
        if (Join(player))
            numberOfClient++;

    }
    return true;
}

void Server::SendToClient(SClient* receiver, string message)
{
    if (send(*receiver->GetSocket(), message.c_str(), message.length(), 0)) {
        OutputDebugStringA("SEND MESSAGE");
        cout << "caca" << endl;
    }
}

void Server::ReceiveFromClient(SClient* emitter)
{
    std::string buffer(4096, 0);
    int num_bytes_received = recv(*emitter->GetSocket(), &buffer[0], buffer.size(), 0);

    if (num_bytes_received != SOCKET_ERROR)
    {
        OutputDebugStringA(buffer.substr(0, num_bytes_received).c_str());
        queueReceive->EnqueueMessage(buffer.substr(0, num_bytes_received), emitter);
    }

    
}


