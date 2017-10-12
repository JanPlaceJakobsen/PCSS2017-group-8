#include <iostream>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main() {


	//initialise winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsData);
	if (wsok != 0) {
		cerr << "can't initialise winsock! Quitting" << endl;
		return;
	}

	//create socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		cerr << "can't create socket" << endl;
	}

	//bind ip and address to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000); //hton = host to netport short who are you!
	hint.sin_addr.S_un.S_addr = INADDR_ANY;	//also look into inet_pton

	bind(listening, (sockaddr*) &hint, sizeof(hint));

	//tell winsock the socket is for listening
	listen(listening, SOMAXCONN);

	//wait for connection
	sockaddr_in client;
	int clientSize = sizeof(client);
	
	//we need a minimum of three of these
	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize); //remote ip address & portnumber, local ip address & portnumber


	char host[NI_MAXHOST];		//client's remote name
	char service[NI_MAXHOST];	//service (i.e. port) the client is connect on

	ZeroMemory(host, NI_MAXHOST);		//same as memset (host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) { //if this equals 0 the host should be connected, otherwise..

		cout << host << " connected on port " << service << endl;

	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	//close listening socket
	closesocket(listening);

	//while loop: accept and echo message back to client
	char buf[4096];

	while (true) {
		ZeroMemory(buf, 4096);

		//wait for client to send data
		int bytesReeived = recv(clientSocket, buf, 4096, 0);
		if (bytesReeived == SOCKET_ERROR) {
			cerr << "error in recv(). quitting" << endl;
			break;
		}
		if (bytesReeived == 0) {
			cout << "client disconnected " << endl;
			break;
		}

		//echo message back to client
		send(clientSocket, buf, bytesReeived + 1, 0);	
	}

	//close socket
	closesocket(clientSocket);

	//cleanup winsock
	WSACleanup();

}