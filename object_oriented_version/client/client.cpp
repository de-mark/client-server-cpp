#include "client.h"

// clientConnect() : Runs chat from the client end
void Client::clientConnect(void){
    char message[3000];

    // gethostbyname takes an IP or name and returns a hostent struct
    struct hostent* host = gethostbyname(this->serverIp);
    sockaddr_in sendSocketAddress;
    bzero((char*)&sendSocketAddress, sizeof(sendSocketAddress));

    // sin_family is the type of address; most internet-based addresses use AF_INET
    sendSocketAddress.sin_family = AF_INET;

    // sin_addr is used for the IP address; s_addr is used for the IP address
    // inet_addr() and inet_ntoa() used to convert into a useable address
    sendSocketAddress.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));

    // sin_port is used to set the port
    // htons converts uint PORT to network byte order
    sendSocketAddress.sin_port = htons(this->port);

    // Setting up a socket for client
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);

    int status = connect(clientSd, (sockaddr*) &sendSocketAddress, sizeof(sendSocketAddress));

    if (status < 0) {
        std::cout << "Error connecting to socket" << std::endl;
        exit(0);
    }

    std::cout << "Connected to server" << std::endl;
    std::cout << "(Type 'bye' at any time to quit)" << std::endl;
    std::cout << "----------------------" << std::endl; 
    std::cout << std::endl;
    
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

        if (data == "bye") {
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
}