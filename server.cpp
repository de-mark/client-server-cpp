#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <strings.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>

// Takes in server port parameters when initially run
int main(int argc, char *argv[]){
    
    // We at least need to know what port the server will run on
    // Closes program if this is not provided
    if (argc != 2) {
        std::cerr << "Please provide a port to run the server on." << std::endl;
        exit(0);
    }

    const int PORT = std::atoi(argv[1]);
    char message[3000];

sockaddr_in serveAddress;
    bzero((char*) &serveAddress, sizeof(serveAddress));

    // sin_family is the type of address; most internet-based addresses use AF_INET
    serveAddress.sin_family = AF_INET;

    // sin_addr is used for the IP address; s_addr is used for the IP address
    // htonl converts the uint (in this case INADDR_ANY) from host byte order to network byte order
    // INADDR_ANY is a contant that equals 0
    // If we only wanted to access via localhost, we'd do
    // serveAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    // sin_port is used to set the port
    // htons converts uint PORT to network byte order
    serveAddress.sin_port = htons(PORT);


    // Open TCP (stream) socket
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSd < 0) {
        std::cerr << "Error in opening server socket" << std::endl;
        exit(0);
    }

    // BIND socket to local address
    int bindStatus = bind(serverSd, (struct sockaddr*) &serveAddress, sizeof(serveAddress));

    if (bindStatus < 0) {
        std::cerr << "Could not bind socket to local address" << std::endl;
        exit(0);
    }

    std::cout << "CHAT SERVER UP AND RUNNING!" << std::endl;
    std::cout << "Waiting for client to connect..." << std::endl << std::endl;

    // Server listens for a maximum of 3 requests at once
    listen(serverSd, 3);

    // RECEIVE request from client
    // A new address is required to connect with the client
    sockaddr_in newSocketAddress;
    socklen_t newSocketAddressSize = sizeof(newSocketAddress);

    // ACCEPT
    // New socket connector created to handle client
    int newSd = accept(serverSd, (sockaddr *) &newSocketAddress, &newSocketAddressSize);

    if (newSd < 0) {
        std::cerr << "Error accepting request from client" << std::endl;
    }

    std::cerr << "Client connected!" << std::endl;

    // Get session time
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);

    // Keep track of data read / written
    int bytesRead = 0, bytesWritten = 0;

    while (1) {
        std::cout << "Waiting for client to respond" << std::endl;
        memset(&message, 0, sizeof(message));
        bytesRead += recv(newSd, (char*) &message, sizeof(message), 0);

        if (!strcmp(message, "exit")) {
            std::cout << "Client has left the chatroom" << std::endl;
            break;
        }

        std::cout << "CLIENT: " << message << std::endl;
        std::cout << ">";

        std::string data;
        std::getline(std::cin, data);
        memset(&message, 0, sizeof(message));
        strcpy(message, data.c_str());

        if (data == "exit") {
            send(newSd, (char*)&message, strlen(message), 0);
            break;
        }

        bytesWritten += send(newSd, (char*)&message, strlen(message), 0);
    }

    gettimeofday(&end1, NULL);

    // Close to prevent zombie instances
    close(newSd);
    close(serverSd);

    std::cout << "CHAT SERVER SUMMARY" << std::endl;
    std::cout << "Started:" << start1.tv_usec << std::endl;
    std::cout << "Duration: " << (end1.tv_sec - start1.tv_sec) << "seconds" << std::endl; 
    std::cout << "Bytes Written: " << bytesWritten << std::endl;
    std::cout << "Bytes Read: " << bytesRead << std::endl << std::endl;

    std::cout << "Closing chat" << std::endl;

    return 0;
}