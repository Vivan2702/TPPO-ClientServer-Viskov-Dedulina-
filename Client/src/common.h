#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream> 
#include <cstdio> 
#include <cstring> 
#include <winsock2.h> 
#pragma comment(lib, "WS2_32.lib")
using namespace std;

DWORD WINAPI clientReceive(LPVOID lpParam) { 

	char buffer[1024] = { 0 };
	SOCKET server = *(SOCKET*)lpParam;
	
	while (true) {
		if (recv(server, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
			cout << "Ошибка подключения к серверу " << WSAGetLastError() << endl;
			return -1;
		}
		if (strcmp(buffer, "Close_Dialog\n") == 0) { 
			cout << "Клиент отключился" << endl;
			break;
		}
		cout << "Собеседник - " << buffer << endl;
		memset(buffer, 0, sizeof(buffer));
	}
	return 1;
}

DWORD WINAPI clientSend(LPVOID lpParam) { 

	char buffer[1024] = { 0 };
	SOCKET server = *(SOCKET*)lpParam;
	
	while (true) {
		fgets(buffer, 1024, stdin);
		if (send(server, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
			cout << "Ошибка отправка на сервер" << WSAGetLastError() << endl;
			return -1;
		}
		if (strcmp(buffer, "Close_Dialog\n") == 0) { 
			cout << "Клиент отключился" << endl;
			break;
		}
	}
	return 1;
}
