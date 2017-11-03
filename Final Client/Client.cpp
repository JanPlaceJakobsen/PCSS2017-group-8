#undef UNICODE

//Define these to ignore winsock errors
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <thread>
#include <winsock2.h>
#include <string>
#include <iostream>
#include "SnailGame.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#define PORT 27015
#define DEFAULT_BUFFER_SIZE 256
#define DEFAULT_SLEEP_SIZE std::chrono::nanoseconds(5000)

//bool game = false;

//Function for printing out an error and defining where in the code it's met
char* ErrorMessage(char*, int);

//Function for checking if two char arrays are alike
bool cstrCheck(const char*, const char*);

/*Currently not in use...*/
////Function for recieving messeges over thread
//void recieveMSG(SOCKET);

int main() {

	//start winsock
	InitWinsock();

	//initialize client socket
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, NULL);

	//Define the type of address to be handled
	sockaddr_in ServerAddress;
	int sizeofAddress = sizeof(ServerAddress);
	ServerAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); //So far the local IP address, needs to be changed later for multi-computer use
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(PORT);

	//Connects the client socket with the server on the specified address + checks for errors
	int check = connect(clientSocket, (struct sockaddr*)&ServerAddress, sizeof(struct sockaddr));
	if (check == SOCKET_ERROR) {
		closesocket(clientSocket);
		ErrorMessage("conect", SOCKET_ERROR);
	}

	//Recieve a name from server as you are connected (e.g. Client0, Client1...)
	char catchName[DEFAULT_BUFFER_SIZE];
	int bytes = recv(clientSocket, catchName, sizeof(catchName), NULL);
	std::string name;
	name.append(catchName, catchName + bytes);
	int iName = stoi(std::to_string(name[6]))-48;

	/*currently not in use...*/
	////Creates a thread for handling messages, so the message stream is automatically updated at all times. Check function "recieveMsg"
	//std::thread t(recieveMsg, clientSocket);
	////Detach thread to run the rest of the code simultaneously with the trhead
	//t.detach();

	//while (!game) {

	//	//take user input string and convert to tangible letter format
	//	std::string message = "";
	//	std::getline(std::cin, message);

	//	//If message equals to "end", then break the loop and end program
	//	if (message == "end") exit(0);

	//	//Create the message with specifics to be send to server
	//	std::string msg = "\n"; msg += name; msg += message; msg += "\n";

	//	//send message (msg) to server usign the client socket
	//	send(clientSocket, msg.data(), msg.size(), NULL);
	//}

	std::this_thread::sleep_for(DEFAULT_SLEEP_SIZE);
	int k = 0;
	while (true) {

		if (findWinner(iName)) break;

		char buf[DEFAULT_BUFFER_SIZE];
		recv(clientSocket, buf, sizeof(buf), NULL);
		buf[DEFAULT_BUFFER_SIZE - 1] = '\0';

		//Run game code
		if (cstrCheck(buf, "/your turn")) {
			//The string for what happened with the snail
			printf("AHA!\n");
			std::string turn = yourTurn(iName);
			char msg[DEFAULT_BUFFER_SIZE];
			strcpy(msg, turn.c_str());

			//Send index name
			send(clientSocket, msg, sizeof(msg), NULL);
			std::this_thread::sleep_for(DEFAULT_SLEEP_SIZE);

			char buffer3[DEFAULT_BUFFER_SIZE] = "/thrown dice";
			send(clientSocket, buffer3, DEFAULT_BUFFER_SIZE, NULL);

			std::this_thread::sleep_for(DEFAULT_SLEEP_SIZE);

			std::string Die = std::to_string(diceThrow);
			send(clientSocket, Die.data(), Die.size(), NULL);

			std::this_thread::sleep_for(DEFAULT_SLEEP_SIZE);

			std::string tempName = std::to_string(iName);
			send(clientSocket, tempName.data(), tempName.size(), NULL);

			std::this_thread::sleep_for(DEFAULT_SLEEP_SIZE);

			char buffer2[DEFAULT_BUFFER_SIZE] = "/end turn";
			send(clientSocket, buffer2, DEFAULT_BUFFER_SIZE, NULL);

			if (findWinner(iName-48))break;

			continue;
		}

		if (cstrCheck(buf, "/thrown dice")) {
			char recvDie[DEFAULT_BUFFER_SIZE];
			recv(clientSocket, recvDie, DEFAULT_BUFFER_SIZE, NULL);

			printf("hit%d", k++);

			char recvName[DEFAULT_BUFFER_SIZE];
			int checkBytes = recv(clientSocket, recvName, DEFAULT_BUFFER_SIZE, NULL);

			printf("hit%d", k++);

			std::string tempname;
			tempname.append(recvName, recvName + checkBytes);
			int index = stoi(std::to_string(tempname[0]))-48;
			pos[index] = pos[index] + (stoi(std::to_string(recvDie[0]))-48);
			continue;
		}
		
		printf(buf);

	}

	printf("\n\n\n\n\n\nGame is done, thanks for playing!\n\n");

	//runs cleanup up socket and winsock
	closesocket(clientSocket);
	WSACleanup();
	system("PAUSE");
	return 0;
}

//initialize winsock
void InitWinsock()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

//function summary = function displays an error and its location in the code, cleans up the winsock and exits on error code
char* ErrorMessage(char* place, int errorCode) {

	//Prints out error message using the arguments
	std::string message = "Error happened in: \'" + (std::string)place + "\' with error code : " + std::to_string(errorCode);
	const char* errorm = message.c_str();
	printf(errorm);

	//cleanup winsock
	WSACleanup();
	system("PAUSE");

	//Exit 1 returns a error on exit, while exit 0 doesn't
	exit(1);
}

bool cstrCheck(const char* a, const char* b) {
	for (int i = 0; i < sizeof(b); i++) {
		if (a[i] != b[i]) {
			return false;
		}
	}
	return true;
}


////function summar = function recieves data from server and displays them in console. used in thread
//void recieveMsg(SOCKET connection) {
//	while (true) {
//
//		//Creates buffer and makes it recieve data from server
//		char recvBuf[DEFAULT_BUFFER_SIZE];
//		int Bytes = recv(connection, recvBuf, sizeof(recvBuf), NULL);
//
//		//If bytes recieved equals to 0, then the server has disconnected
//		if (Bytes == 0) {
//			printf("\n <Connection closed> \n");
//			break;
//		}
//
//		//Captures data in a string
//		std::string message;
//		message.append(recvBuf, recvBuf + Bytes);
//
//		if (cstrCheck(recvBuf, "/end client")) {
//			exit(0);
//		}
//
//		if (cstrCheck(recvBuf, "/start game")) {
//			printf("starting game", "\n");
//			game = true;
//			break;
//		}
//
//		printf(message.c_str(), "\n");
//		std::this_thread::sleep_for(std::chrono::nanoseconds(500));
//	}
//}

