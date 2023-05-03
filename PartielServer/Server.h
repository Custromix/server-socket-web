#pragma once
class Server
{
public:
	Server(int port, int slot, HWND hwnd);
	~Server();

	void Start();
	void Listen();
	void Stop();

	CResult<int>* IsExist(SClient* client);

	bool Join(SClient* joiningClient);
	bool Connect(WPARAM wParam);
	bool Disconnect(WPARAM wParam);

	void SendToClient(SClient* receiver, string message);
	void ReceiveFromClient(SClient* receiver);


	vector<SClient*>* GetClients()								{ return &connectedClient; }
	SOCKET* GetServerSocket()									{ return &serverSocket; }
	HWND* GetWindow()											{ return &WINDOW; }

	int GetSlot()												{ return slot_; }
	void SetSlot(int slot)										{ slot_ = slot; }

	QueueMessageManager* GetReceivedMessage()					{ return queueReceive; }
	QueueMessageManager* GetSendedMessage()						{ return queueWrite; }


private :
	int slot_;
	vector<SClient*> connectedClient;

	int PORT;
	HWND WINDOW;

	WSADATA wsaData;
	SOCKET serverSocket;
	SOCKADDR_IN serverAddr;

	QueueMessageManager* queueReceive;
	QueueMessageManager* queueWrite;
};