#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

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

// Takes in client ip and port parameters when initially run
int main(int argc, char *argv[]){
    // We need ip address AND port for the client
    if (argc != 3) {
        std::cerr << "Please provide (1) ip address and (2) port to run chat client." << std::endl;
        exit(0);
    }

    const char *SERVER_IP = argv[1]; 
    const int PORT = std::atoi(argv[2]);
    char message[3000];

    // gethostbyname takes an IP or name and returns a hostent struct
    struct hostent* host = gethostbyname(SERVER_IP);
    sockaddr_in sendSocketAddress;
    bzero((char*)&sendSocketAddress, sizeof(sendSocketAddress));

    // sin_family is the type of address; most internet-based addresses use AF_INET
    sendSocketAddress.sin_family = AF_INET;

    // sin_addr is used for the IP address; s_addr is used for the IP address
    // inet_addr() and inet_ntoa() used to convert into a useable address
    sendSocketAddress.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));

    // sin_port is used to set the port
    // htons converts uint PORT to network byte order
    sendSocketAddress.sin_port = htons(PORT);

    // Setting up a socket for client
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);

    int status = connect(clientSd, (sockaddr*) &sendSocketAddress, sizeof(sendSocketAddress));

    if (status < 0) {
        std::cout << "Error connecting to socket" << std::endl;
        exit(0);
    }

    std::cout << "Connected to server" << std::endl;
    
    // Keep track of bytes read and written
    int bytesRead = 0, bytesWritten = 0;
    
    // Keeping track of time for summaries
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);

    while (1) {
        std::cout << "YOU: > ";
        std::string data;
        std::getline(std::cin, data);
        memset(&message, 0, sizeof(message));
        strcpy(message, data.c_str());

        if (data == "exit") {
            send(clientSd, (char*)&message, strlen(message), 0);
            break;
        }

        bytesWritten += send(clientSd, (char*) &message, strlen(message), 0);
        std::cout << "\nWaiting for server response" << std::endl;
        memset(&message, 0, sizeof(message));

        bytesRead += recv(clientSd, (char*)&message, sizeof(message), 0);

        if (!strcmp(message, "exit")) {
            std::cout << "\nServer has closed chat room" << std::endl;
            break;
        } 

        std::cout << "SERVER: " << message << std::endl;

    }

    gettimeofday(&end1, NULL);
    close(clientSd);

    std::cout << std::endl << std::endl;
    std::cout << "CHAT CLIENT SUMMARY" << std::endl;
    std::cout << "----------------------" << std::endl; 
    std::cout << "Started:" << start1.tv_usec << std::endl;
    std::cout << "Duration: " << (end1.tv_sec - start1.tv_sec) << "seconds" << std::endl; 
    std::cout << "Bytes Written: " << bytesWritten << std::endl;
    std::cout << "Bytes Read: " << bytesRead << std::endl << std::endl;

    std::cout << "Closing chat" << std::endl;

    return 0;
}