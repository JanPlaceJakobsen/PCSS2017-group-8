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
		cerr << "can't create socket boiiii!!!\n ALL HAIL THE FAB BOI!!!" << endl;
	}

	//bind ip and address to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000); //hton = host to netport short who are you!
	hint.sin_addr.S_un.S_addr = INADDR_ANY;	//also look into inet_pton



	//tell winsock the socket is for listening

	//wait for connection

	//close listening socket

	//while loop: accept and echo message back to client

	//close socket

	//shutdown winsock

}