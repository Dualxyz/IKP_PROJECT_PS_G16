#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "../Common/Structs.h"
#pragma comment(lib, "ws2_32.lib")

#define MAX_MESSAGE_LENGTH 100
#define WIN32_LEAN_AND_MEAN
#define DEFAULT_BUFLEN 524
#define DEFAULT_PORT 27016


bool InitializeWindowsSockets();
int SendTopic(void * topic, void *message);
int Connect();

// socket used to communicate with server
SOCKET connectSocket = INVALID_SOCKET;

//connect to server DONE
int Connect() {
	// create a socket
	connectSocket = socket(AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP);

	if (connectSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// create and initialize address structure
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(DEFAULT_PORT);
	// connect to server specified in serverAddress and socket connectSocket
	if (connect(connectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		printf("Unable to connect to server.\n");
		closesocket(connectSocket);
		WSACleanup();
	}

	return 0;

}

//init DONE
bool InitializeWindowsSockets()
{
	WSADATA wsaData;
	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

//msg to be sent to subscribers
void pMsg(char* newMessage)
{
	memset(newMessage, 0, MAX_MESSAGE_LENGTH);
	printf("Enter new message for selected topic: ");
	while (newMessage[0] == 0 || strlen(newMessage) == 0 || newMessage[0] == 10) {

		fgets(newMessage, MAX_MESSAGE_LENGTH, stdin);
		fflush(stdin);
	}
}
//send msg to server q PROMENI OVO NAKON STRUKTURA
int SendTopic(void * topic, void *message) {

	//ubaci topic i poruku u strukturu
	FULLMESSAGE vest;
	strcpy_s(vest.topic, (char*)topic);
	strcpy_s(vest.text, (char*)message);

	// Send an prepared message with null terminator included
	int iResult = send(connectSocket, (char *)&vest, (int)sizeof(FULLMESSAGE), 0);


	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return -1;
	}
	return iResult;
}


