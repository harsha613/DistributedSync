#include "../include/network.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

namespace sync {

    Network::Network() : socketFd(-1), connected(false) {}

    Network::~Network() {
        disconnect();
    }

    bool Network::connect(const std::string& ip, uint16_t port) {
        if (connected) {
            std::cerr << "Already connected." << std::endl;
            return true;
        }

        socketFd = socket(AF_INET, SOCK_STREAM, 0);
        if (socketFd < 0) {
            std::cerr << "Failed to create socket." << std::endl;
            return false;
        }

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        if(inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
            std::cerr << "Failed to convert IP address." << std::endl;
            close(socketFd);
            socketFd = -1;
            return false;
        }

        if (::connect(socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Failed to connect to server." << std::endl;
            close(socketFd);
            socketFd = -1;
            return false;
        }

        connected = true;
        return true;
    }

    void Network::disconnect() {
        if(!connected) return;
        close(socketFd);
        socketFd = -1;
        connected = false;
    }

    bool Network::isconnected() const {
        return connected;
    }
}