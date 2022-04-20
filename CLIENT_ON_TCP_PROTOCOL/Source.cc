#include <iostream>
#include <string>
#include <ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib")


int main() {
	std::string ipAddress = "127.0.0.1";	
	int port = 54000;				

	WSAData windosckData;
	WORD ver = MAKEWORD(2, 2);
	int winsockResult = WSAStartup(ver, &windosckData);
	if (winsockResult != 0) {
		std::cerr << "Can`t start winsock " << winsockResult << "\n";
		return 1;
	}

	SOCKET winsock = socket(AF_INET, SOCK_STREAM, 0);
	if (winsock == INVALID_SOCKET) {
		std::cerr << "Can`t create socket, error: " << WSAGetLastError() << "\n";
		WSACleanup();
		return 1;
	}

	sockaddr_in Bind;
	Bind.sin_family = AF_INET;
	Bind.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &Bind.sin_addr);

	int connResult = connect(winsock, (sockaddr*)&Bind, sizeof(Bind));
	if (connResult == SOCKET_ERROR) {
		std::cerr << "Can`t connect to the server, error: " << WSAGetLastError() << "\n";
		closesocket(winsock);
		WSACleanup();
		return 1;
	}

	char buf[4096];
	std::string userInput;

	do {
		std::cout << "> ";
		std::getline(std::cin, userInput);

		if (userInput.size() > 0) {	
			int sendResult = send(winsock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) {

				ZeroMemory(buf, 4096);
				int bytesReceived = recv(winsock, buf, 4096, 0);
				if (bytesReceived > 0) {

					std::cout << "Server> " << std::string(buf, 0, bytesReceived) << "\n";
				}

			}

		}


	} while (userInput.size() > 0);
	closesocket(winsock);
	WSACleanup();
}