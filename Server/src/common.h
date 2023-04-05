#pragma once
#pragma comment(lib, "WS2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream> 
#include <cstdio> 
#include <cstring> 
#include <winsock2.h> 
#include <thread>
#include "string.h"

using namespace std;

DWORD WINAPI serverReceive(LPVOID lpParam) {

	char login[64] = { 0 };
	char buffer[1024] = { 0 };
	SOCKET client = *(SOCKET*)lpParam;
	if (recv(client, login, sizeof(login), 0) == SOCKET_ERROR) {
		cout << "Ошибка, логин не получен" << WSAGetLastError() << endl;
		return -1;
	}
	while (true) {

		if (recv(client, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {

			cout << "Ошбика принятия данных" << WSAGetLastError() << endl;
			return -1;
		}
		if (strcmp(buffer, "Close_Dialog\n") == 0) {
			cout << "Произошло отключение клиента " << login << endl;
			continue;
		}
		cout << "Клиент " << login << " - " << buffer << endl;
		memset(buffer, 0, sizeof(buffer)); //Очистить буфер
	}
	return 1;
}

DWORD WINAPI serverSend(LPVOID lpParam) {

	char buffer[1024] = { 0 };
	SOCKET client = *(SOCKET*)lpParam;
	while (true) {
		fgets(buffer, 1024, stdin);
		if (send(client, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
			cout << "Ошибка отправки данных " << WSAGetLastError() << endl;
			return -1;
		}
		if (strcmp(buffer, "Close_Dialog\n") == 0) {
			cout << "Клиент отключился" << endl;
			continue;
		}
	}
	return 1;
}