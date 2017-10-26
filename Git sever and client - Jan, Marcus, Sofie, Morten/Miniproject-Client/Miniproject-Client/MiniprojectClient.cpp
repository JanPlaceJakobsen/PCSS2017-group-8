//Client

#include <Windows.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

SOCKET sock; //This is the socket that we will use.
SOCKADDR_IN i_sock; //This will contain some information about our socket.
WSADATA Data; //This is to save our socket version

int Connect(char *IP, int Port) // this is our fuction
{
	WSAStartup(MAKEWORD(2, 2), &Data);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	// vend tilbage til 0 senere
	if (sock == INVALID_SOCKET)
	{
	
		return 1;
	}

	i_sock.sin_family = AF_INET; // Address family spec (IPv4).
	i_sock.sin_addr.s_addr = inet_addr(IP); 
	i_sock.sin_port = htons(Port); //This is to set the Server port.
	int ss = connect(sock, (struct sockaddr *)&i_sock, sizeof(i_sock));
	if (ss != 0)
	{
		printf("Cannot connect");
		return 0;
	}

	printf("Ohoy");
	return 1;

	/*
	INADDR_ANY is replaced by inet_addr(IP).

	The bind, accept and listen functions have been removed.
	*/

	//Send and recive data in the socket
	int slen;
	slen = send(sock, Buf, len, 0);
	if (slen < 0)
	{
		printf("cannot send data");
		return 1;
	}
	return slen;

	int slen;
	slen = recv(sock, Buf, len, 0);
	if (slen < 0)
	{
		printf("cannot recive");
		return 1;
	}
	return slen;

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
		send((char *)&packet, sizeof(packet), 1); // The function that we have created to send data.
	}
	/*
	This is the code for sending to i number of clients.
	*/

	for (int i = 0; i < 4; i++)
	{
		MyPacket packet; //This is the struct description
		recv((char *)&packet, sizeof(packet), i); // The function that we have created to send data.
	}

	/*
	This is the code for reciving to i number of clients.
	*/

	//End the socket
	int EndSocket();
	{
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	/*
	Sock is the socket that was created in the top using the socket function.
	WSACleanup is used to end the socket and clean everything.
	*/








