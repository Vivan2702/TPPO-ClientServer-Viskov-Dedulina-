#include "common.h"

int main() {
	setlocale(LC_ALL, "Russian");
	WSADATA WSAData;
	SOCKET server;
	SOCKADDR_IN addr;
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	if ((server = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		cout << "Ошибка создания сокета" << WSAGetLastError() << endl;
		return -1;
	}

	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5555); 
	if (connect(server, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		cout << "Ошибка подключения к серверу" << WSAGetLastError() << endl;
		return -1;
	}
	cout << "Соединение с сервером установленно" << endl;
	cout << "Для отключения от сервера и выхода из диалога, напишите \"Close_Dialog\"" << endl;
	DWORD tid;
	HANDLE t1 = CreateThread(NULL, 0, clientReceive, &server, 0, &tid);
	if (t1 == NULL) cout << "Ошибка создания потока" << GetLastError();
	HANDLE t2 = CreateThread(NULL, 0, clientSend, &server, 0, &tid);
	if (t2 == NULL) cout << "Ошибка создания потока" << GetLastError();

	WaitForSingleObject(t1, INFINITE);
	WaitForSingleObject(t2, INFINITE);

	closesocket(server);
	WSACleanup();
}