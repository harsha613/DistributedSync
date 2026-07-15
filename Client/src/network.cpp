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

    bool Network::sendAll(const void* data, std::size_t size) {

        const uint8_t* buffer = static_cast<const uint8_t*>(data);

        std::size_t totalSent = 0;
        while (totalSent < size) {

            ssize_t bytesSent = send(socketFd, buffer + totalSent, size - totalSent, 0);

            if (bytesSent < 0) {
                std::cerr << "Failed to send data." << std::endl;
                return false;
            }
            totalSent += static_cast<std::size_t>(bytesSent);
        }
        return true;
    }

    bool Network::receiveAll(void* data, std::size_t size) {

        uint8_t* buffer = static_cast<uint8_t*>(data);

        std::size_t totalReceived = 0;
        while (totalReceived < size) {

            ssize_t bytesReceived = recv(socketFd, buffer + totalReceived, size - totalReceived, 0);

            if (bytesReceived < 0) {
                std::cerr << "Failed to receive data or connection closed." << std::endl;
                disconnect();
                return false;
            }

            if (bytesReceived == 0) {
                std::cerr << "Connection closed." << std::endl;
                disconnect();
                return false;
            }
            totalReceived += static_cast<std::size_t>(bytesReceived);
        }
        return true;
    }

    bool Network::sendPacket(Command command, const std::vector<uint8_t>& payload) {

        PacketHeader header;
        header.command = htons(static_cast<uint16_t>(command));
        header.reserved = htons(0);
        header.payloadSize = htonl(static_cast<uint32_t>(payload.size()));
        header.requestId = htonll(0); // For simplicity, we can set this to 0 for now.

        // Send the header
        if (!sendAll(&header, sizeof(header))) {
            return false;
        }

        // Send the payload
        if (!payload.empty() && !sendAll(payload.data(), payload.size())) {
            return false;
        }

        return true;
    }

    bool Network::receivePacket(PacketHeader& header, std::vector<uint8_t>& payload) {

        // Receive the header
        if (!receiveAll(&header, sizeof(header))) {
            return false;
        }

        // Convert header fields from network byte order to host byte order
        header.command = ntohs(header.command);
        header.reserved = ntohs(header.reserved);
        header.payloadSize = ntohl(header.payloadSize);
        header.requestId = ntohll(header.requestId);

        // Resize the payload vector to hold the incoming data
        payload.resize(header.payloadSize);

        // Receive the payload
        if (header.payloadSize > 0 && !receiveAll(payload.data(), header.payloadSize)) {
            return false;
        }

        return true;
    }
}