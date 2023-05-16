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

struct clients {
	SOCKET cl1;
	SOCKET cl2;
};

DWORD WINAPI serverReceive(void* p) {

	clients * d = ((clients*)p);
	char login[64] = { 0 };
	char buffer[1024] = { 0 };
	SOCKET client1 = d->cl1;
	SOCKET client2 = d->cl2;

	if (recv(client1, login, sizeof(login), 0) == SOCKET_ERROR) {
		cout << "Ошибка, логин не получен" << WSAGetLastError() << endl;
		return -1;
	}
	if (send(client2, login, sizeof(buffer), 0) == SOCKET_ERROR) {
		cout << "Ошибка отправки данных " << WSAGetLastError() << endl;
		return -1;
	}
	while (true) {

		if (recv(client1, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {

			cout << "Ошбика принятия данных" << WSAGetLastError() << endl;
			return -1;
		}
		if (strcmp(buffer, "Close_Dialog\n") == 0) {
			cout << "Произошло отключение клиента " << login << endl;
			continue;
		}
		login[strlen(login) - 1] = ' ';
		buffer[strlen(buffer) - 1] = ' '; // убирание перевода строки для нормального вывода
		if (strlen(buffer) != 0)
		{
			cout << "Клиент " << login << " - " << buffer << endl;
		}
		if (send(client2, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
			cout << "Ошибка отправки данных " << WSAGetLastError() << endl;
			return -1;
		}
		memset(buffer, 0, sizeof(buffer)); //Очистить буфер
		
	}
	return 1;
}

DWORD WINAPI serverSend(void* p) {
	clients * d = (clients*)p;
	char buffer[1024] = { 0 };
	SOCKET client = d->cl1;
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