#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

string getCatImageURL() {
    return "https://cataas.com/cat";
}

void handleClient(SOCKET clientSocket) {
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        string url = getCatImageURL();

        string filename = "images/cat_" + to_string(time(nullptr)) + ".jpg";
        string command = "curl -L \"" + url + "\" -o \"" + filename + "\"";

        cout << "Downloading: " << url << " -> " << filename << endl;

        int result = system(command.c_str());

        if (result == 0) {
            cout << "Image saved: " << filename << endl;

            ofstream log("log.txt", ios::app);
            log << filename << " downloaded from " << url << endl;
            log.close();
        }
        else {
            cout << "Failed to download image\n";
        }
    }

    closesocket(clientSocket);
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5555);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    cout << "Server listening on port 5555...\n";

    system("mkdir images");

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        thread(handleClient, clientSocket).detach();
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
