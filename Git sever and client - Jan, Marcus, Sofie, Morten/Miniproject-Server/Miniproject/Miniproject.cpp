//Server
#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

SOCKET sock; //This is the socket that we will use.
SOCKET sock2[200]; //This is the sockets that will be recived from the Clients and sended to them.
SOCKADDR_IN i_sock2; //This will contain information about the clients connected to the server.
SOCKADDR_IN i_sock; //This will contain some information about our socket.
WSADATA Data; //This is to save our socket version
int clients = 0; //We will use it in the accepting clients.

int StartServer(int Port) // this is our fuction
{
	int err;
	WSAStartup(MAKEWORD(2, 2), &Data); 
	sock = socket(AF_INET, SOCK_STREAM, 0); 
	// vend tilbage til 0 senere
	if (sock == INVALID_SOCKET) 
	{
		Sleep(4000);
		exit(0);
		return 0;
	}
	/*
	WSAStartup is to Initialize the socket and set our socket version. 
	MAKEWORD is how we packedge our bits. 
	&Data is to save the socket version.

	Socket type. AF_INET is the address family spec (IPv4). 
	Sock_Stream is the stream of data of the socket (there are four types).
	*/

	i_sock.sin_family = AF_INET; // Address family spec (IPv4).
	i_sock.sin_addr.s_addr = htonl(INADDR_ANY); //Is to start the server at your IP
	i_sock.sin_port = htons(Port); //This is to set the Server port.
	err = bind(sock, (LPSOCKADDR)&i_sock, sizeof(i_sock));
	if (err != 0)
	{
		return 0;
	}

	/*
	sock is the socket that was created. 
	LPSOCKADDR&i_sock is the information about the socket. 
	Sizeof is the size of the socket information.
	*/

	err = listen(sock, 2); 
	if (err == SOCKET_ERROR)
	{

		return 0;
	}
	/*
	The sock if the socket that was created in the socket function. 
	2 is the maximum number of clients supported.
	*/

	// The accept function. This is to accept incomming connections.
	while (1)
	{
		for (int i = 0; i < 4; i++)
		{
			if (clients < 4)
			{
				int so2len = sizeof(i_sock2);
				sock2[clients] = accept(sock, (sockaddr *)&i_sock2, &so2len); 
				if (sock2[clients] == INVALID_SOCKET)
				{
					return 0;
				}
				printf("A client has joined the server (IP: %s)\n", i_sock2.sin_addr.s_addr);
				clients++;
				continue;
			}
			else
			{
				break;
			}
		}
	}
	return 1;
}
/* 
sock is the socket that was created in the socket function.
(Sockaddr *)&i_sock2 is the informtaion of the accepted client. 
so2len is the size of the information about the accepted client. 
*/


// Send fuction
int Send(char *Buf, int len, int Client)
{
	slen = send(sock2[Client], Buf, len, 0);
	if (slen < 0)
	{
		printf("Cannot send data!");
		return 1;
	}
	return slen;
}
/*
The int len is the size of the data that will be sent in char *Buf
And the int client is the client ID that the data will be sent to him

In the send fuction there is:
sock2[Client] accepts the client ID.
Buf is the data that will be sent.
Len is the size of the data that wil be sent.
0 is the set of flags that specify the way in which the call is made. (UDP port)
*/


//Recive function.
int Recive(char *Buf, int len, int Client)
{
	slen = recv(sock2[Client], Buf, len, 0);
	if (slen < 0)
	{
		printf("Cannot send data!");
		return 1;
	}
	return slen;
}
/*
The int len is the size of the data that will be recived in char *Buf
And the int client is the client ID that the data will be recived by him

In the recived fuction there is:
sock2[Client] accepts the client ID.
Buf is the data that will be recived.
Len is the size of the data that wil be recived.
0 is the set of flags that specify the way in which the call is made. (UDP port)
*/

//Exchange packets
struct MyPacket
{
	int mylong;
	char mystring[256];
};
/*
This is my packet code
*/
for (int i = 0; i < 4; i++)
{
	MyPacket packet; //This is the struct description
	Send((char *)&packet, sizeof(packet), 1); // The function that we have created to send data.
}
/*
This is the code for sending to i number of clients.
*/

for(int i = 0; i < 4; i++)
{
	MyPacket packet; //This is the struct description
	Recive((char *)&packet, sizeof(packet), i); // The function that we have created to send data.
}

/*
This is the code for reciving to i number of clients.
*/

//End the socket
int EndSocket()
{
	closesocket(sock);
	WSACleanup();
	return 1;
}
/*
Sock is the socket that was created in the top using the socket function.
WSACleanup is used to end the socket and clean everything.
*/