#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    string serverIp = "127.0.0.1";
    int port = 5555;

    cout << "Press ENTER to request a new image. Ctrl+C to exit.\n";

    while (true) {
        cin.get();

        SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr);

        connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));

        string msg = "GET_IMAGE";
        send(sock, msg.c_str(), msg.size(), 0);

        closesocket(sock);
    }

    WSACleanup();
    return 0;
}
