#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

#define PORT 54000



using namespace std;

void main() {
	//initiliaze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int ws0k = WSAStartup(ver, &wsData);

	if (ws0k != 0) {
		cerr << "Can't Initiliaze Winsock" << endl;
		return;
	}

	//create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		cerr << "Can't Create a Socket" << endl;
		return;
	}

	//bind the socket to an IP Address and PORT
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&addr, sizeof(addr));

	//winsock listeing
	listen(listening, SOMAXCONN);

	//waiting the connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];		// client remote name
	char service[NI_MAXHOST];   // service client (such as port)

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		cout << host << " Connected on PORT " << service << endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " Connected on PORT " << ntohs(client.sin_port) << endl;
	}

	//close listening socket
	closesocket(listening);

	char buf[4096];

	while (true) {
		ZeroMemory(buf, 4096);
		//wait the client send the data

		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR) {
			cerr << "Error in received... Quitting" << endl;
			break;
		}

		if (bytesReceived == 0) {
			cout << "Client Disconnected!" << endl;
			break;
		}
		//message back to client
		send(clientSocket, buf, bytesReceived + 1, 0);
	}
	//close the socket
	closesocket(clientSocket);
	//cleanup winsock
	WSACleanup();
}
