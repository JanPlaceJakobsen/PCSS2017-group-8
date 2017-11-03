#undef UNICODE

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <thread>
#include <winsock2.h>
#include <string>
#include <iostream>
#include <chrono>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#define PORT 27015
#define MAX_CLIENTS 3
#define DEFAULT_BUFFER_SIZE 256

//Creates an array of sockets to hold clients
SOCKET clients[MAX_CLIENTS];
//std::thread t1[MAX_CLIENTS]; //Was used for handling lobby's for communication!
int numbclients = 0;

//Sends error messages and displays where in the code they are localiced, as well as ending the code with an error
char* ErrorMessage(char*, int);

//initialize Winsock
void InitWinsock();

//Function for checking whether or not two char arrays are the same; Returns true if they are.
bool cstrCheck(const char*, const char*);



int main() {

	//start winsock
	InitWinsock();

	//Define the type of address to be handled
	sockaddr_in address;
	int sizeofAddress = sizeof(address);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);

	//Create listening socket.
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, NULL); //AF_INET uses IPV4 and SOCK_STREAM defines a two way connection (TCP)
	if (listenSocket == INVALID_SOCKET) ErrorMessage("SOCKET s", INVALID_SOCKET);

	//Bind the socket. Not using namespace is due to this, the std::bind function overwrites this function, so you need to specify the difference.
	bind(listenSocket, (SOCKADDR*)&address, sizeof(address));

	//Listen on listen socket
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		closesocket(listenSocket);
		ErrorMessage("Listen", SOCKET_ERROR);
	}

	//accept clients (Size of clients = max number)
	SOCKET acceptSocket;
	for (int i = 0; i < MAX_CLIENTS; i++) {

		//Accepts incoming clients' connection on the listen socket and checks for errors
		acceptSocket = accept(listenSocket, (sockaddr*)&address, &sizeofAddress);
		if (acceptSocket == 0) {
			closesocket(listenSocket);
			closesocket(acceptSocket);
			ErrorMessage("accept", WSAGetLastError());
		}

		//Send client name directly to client
		std::string msg = "client" + std::to_string(numbclients) + ":";
		send(acceptSocket, msg.data(), msg.size(), NULL);
		numbclients++;

		//assign the accepted socket to the clients array
		clients[i] = acceptSocket;

		////Create thread for communication over the newly connected socket
		//t1[i] = std::thread(lobby, i);	//Run a lobby for all connections, which indepentadly checks for updated messages.
		//t1[i].detach();
		//numbclients++;

		//std::string newmsg = "Welcome to the server!\nwe're currently waiting for " + std::to_string(MAX_CLIENTS - numbclients) + " clients for the game to start!\n\nYou are with " + std::to_string(numbclients - 1) + " other clients, write somethign friendly to them in chat! :)\n\n";
		//send(acceptSocket, newmsg.data(), newmsg.size(), NULL);
	}

	//Thread for handling new connections to the server after the limit is met
	std::thread a([&]() {
		while (true) {
			acceptSocket = accept(listenSocket, (sockaddr*)&address, &sizeofAddress);
			if (acceptSocket == 0) {
				closesocket(listenSocket);
				closesocket(acceptSocket);
				ErrorMessage("accept", WSAGetLastError());
			}
			//The first thing send to the client resolves in a username, so some data needs to be send first!
			std::string noName = "NoName";
			send(acceptSocket, noName.data(), noName.size(), NULL);

			//Send a message that the server is regrettably full at the moment!
			std::string msg = "We're incredibly sorry, but as of this moment the game server is full!\nCome back another time :)!\n";
			send(acceptSocket, msg.data(), msg.size(), NULL);

			char buffer[DEFAULT_BUFFER_SIZE] = "/end client";
			send(acceptSocket, buffer, DEFAULT_BUFFER_SIZE, NULL);
		}
	});
	a.detach();

	//Sleep thread: Letting all clients set up
	std::this_thread::sleep_for(std::chrono::nanoseconds(500));

	//Send the message for the game lobby being full, and send command for game to start
	for (int i = 0; i < MAX_CLIENTS; i++) {
		char buf[DEFAULT_BUFFER_SIZE] = "The max number of participants have been met and the chat has\nbeen turned off!\nThe game is ready to start! Everyone go to your snails and prepare for takeof!\n\n";
		send(clients[i], buf, DEFAULT_BUFFER_SIZE, NULL);
		char buffer[DEFAULT_BUFFER_SIZE] = "\nWELCOME TO THE FANTACTIC FANTASTICAL SNAIL RACE EVER IN HISTORY OF ALL TIME AND FOREVER!\n                                   -AND EVER!";
		send(clients[i], buffer, DEFAULT_BUFFER_SIZE, NULL);
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));

	//Tell client 0 whose turn it is
	char buf[DEFAULT_BUFFER_SIZE] = "/your turn";
	send(clients[0], buf, DEFAULT_BUFFER_SIZE, NULL);

	int i = 0;
	while (true) {
		//recieve message
		char buf[DEFAULT_BUFFER_SIZE];
		recv(clients[i], buf, DEFAULT_BUFFER_SIZE, NULL);

		buf[DEFAULT_BUFFER_SIZE - 1] = '\0';

		//If recieved message = "/end turn", send "/your turn"  to next client
		if (cstrCheck(buf, "/end turn")) {
			printf("Ending turn...");
			std::this_thread::sleep_for(std::chrono::seconds(1));
			i = (i + 1) % MAX_CLIENTS;					//Specifies which client is next (cycles between 0 1 and 2)
			char bufer[DEFAULT_BUFFER_SIZE] = "/your turn";
			send(clients[i], bufer, DEFAULT_BUFFER_SIZE, NULL);
			continue;
		}

		//send all information to all other clients!
		for (int j = 0; j < MAX_CLIENTS; j++) {
			if (j == i) continue;
			send(clients[j], buf, DEFAULT_BUFFER_SIZE, NULL);
		}
	}


	printf("Closing down");
	//Closes all client sockets
	for (int i = 0; i < numbclients; i++)
		closesocket(clients[i]);

	//close sockets and run cleanUp!
	closesocket(listenSocket);
	closesocket(acceptSocket);
	WSACleanup();
	system("PAUSE");
	return 0;
}

//Sends error messages and displays where in the code they are localiced, as well as ending the code with an error
char* ErrorMessage(char* place, int errorCode) {
	std::string message = "Error happened in: \'" + (std::string)place + "\' with error code : " + std::to_string(errorCode);
	const char* errorm = message.c_str();
	printf(errorm);
	WSACleanup();
	system("PAUSE");
	exit(1);
}

//initialize winsock
void InitWinsock()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

/*Not currently in use...*/
////A lobby for recieving and sending information between clients. Made for a thread so that it can run for each client independtedly
//void lobby(int index) {
//	while (true) {
//
//		if (numbclients == MAX_CLIENTS) break;
//
//		//Catch information send from client socket [index]. also make it tangible string :D!
//		char* buffer = new char[DEFAULT_BUFFER_SIZE];
//		int check = recv(clients[index], buffer, DEFAULT_BUFFER_SIZE, NULL);
//
//		if (numbclients == MAX_CLIENTS) break;
//
//		//Check for message having data.
//		if (check > 0) {
//			for (int i = 0; i < numbclients; i++) {
//
//				//If the received data is send from the client equal to index, then ignore it
//				if (i == index) continue;
//
//				//Send message to clients
//				std::string msg;
//				msg.append(buffer, buffer + check);
//				send(clients[i], msg.data(), msg.size(), 0);
//			}
//		}
//		//If data is below or equal to 0, then end thread
//		else {
//			std::string buf = "An error happened";
//			send(clients[index], buf.data(), buf.size(), NULL);
//			numbclients--;
//			closesocket(clients[index]);
//			break;
//		}
//	}
//}

bool cstrCheck(const char* a, const char* b) {
	for (int i = 0; i < sizeof(b); i++) {
		if (a[i] != b[i]) {
			return false;
		}
	}
	return true;
}