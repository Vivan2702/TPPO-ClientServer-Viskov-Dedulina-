#include "common.h"



int main(){
	
    setlocale(LC_ALL, "Russian");
    WSADATA WSAData;
	SOCKET server, client1, client2;
    SOCKADDR_IN serverAddr, clientAddr1, clientAddr2;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == INVALID_SOCKET) {
		cout << "Ошибка иницилизации" << WSAGetLastError() << endl;
		return -1;
	}
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(5555);
	if (bind(server, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cout << "Ошибка иницилизации" << WSAGetLastError() << endl;
		return -1;
	}
	if (listen(server, 0) == SOCKET_ERROR) { 
		cout << "Ошибка прослушивания" << WSAGetLastError() << endl;
		return -1;
	}
	cout << "Отслеживание входящих подключений. . . ." << endl;

    char buffer[1024] = { 0 };
    int clientAddrSize1 = sizeof(clientAddr1);
	int clientAddrSize2 = sizeof(clientAddr2);
	
	
    if (((client1 = accept(server, (SOCKADDR*)&clientAddr1, &clientAddrSize1)) != INVALID_SOCKET) && ((client2 = accept(server, (SOCKADDR*)&clientAddr2, &clientAddrSize2)) != INVALID_SOCKET)) {
    
		clients * d = new clients{ client1, client2 };
		clients* d1 = new clients{ client2, client1 };
        cout << "Оба клиента подключены успешно!" << endl;

        DWORD tid1;
        DWORD tid2;
        HANDLE t1 = CreateThread(NULL, 0, serverReceive, (void*)d, 0, &tid1);
		if (t1 == NULL) { 
			cout << "Ошибка создания потока" << WSAGetLastError() << endl;
		}
        HANDLE t2 = CreateThread(NULL, 0, serverReceive, (void*)d1, 0, &tid2);
		if (t1 == NULL) { 
			cout << "Ошибка создания потока" << WSAGetLastError() << endl;
		}
        HANDLE t3 = CreateThread(NULL, 0, serverSend, (void*)d, 0, &tid1);
		if (t2 == NULL) {
			cout << "Ошибка создания потока" << WSAGetLastError() << endl;
		}
        HANDLE t4 = CreateThread(NULL, 0, serverSend, (void*)d1, 0, &tid2);
		if (t2 == NULL) {
			cout << "Ошибка создания потока" << WSAGetLastError() << endl;
		}
        
		WaitForSingleObject(t1, INFINITE);
		WaitForSingleObject(t2, INFINITE);
		WaitForSingleObject(t3, INFINITE);
		WaitForSingleObject(t4, INFINITE);

        closesocket(client1);
        closesocket(client2);
        if (closesocket(server) == SOCKET_ERROR) { //Ошибка закрытия сокета
			cout << "Закрытие сокета произошло с ошибкой" << WSAGetLastError() << endl;
			return -1;
		}
        WSACleanup();
		delete d;
    }
}