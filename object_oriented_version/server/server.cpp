#include "server.h"

int Server::chatSession(void){
    char message[3000];

    std::cout << "Waiting for client to connect..." << std::endl << std::endl;
    // Server listens for a maximum of 3 requests at once
    listen(this->serverSd, 3);

    // RECEIVE request from client
    // A new address is required to connect with the client
    sockaddr_in newSocketAddress;
    socklen_t newSocketAddressSize = sizeof(newSocketAddress);

    // ACCEPT
    // New socket connector created to handle client
    int newSd = accept(this->serverSd, (sockaddr *) &newSocketAddress, &newSocketAddressSize);

    if (newSd < 0) {
        std::cerr << "Error accepting request from client" << std::endl;
        return 0;
    }

    std::cerr << "Client connected!" << std::endl;
    std::cout << "(Type 'bye' at any time to quit)" << std::endl;
    std::cout << "----------------------" << std::endl; 
    std::cout << std::endl;

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
        std::cout << "SERVER: > ";

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
    close(this->serverSd);

    std::cout << std::endl << std::endl;
    std::cout << "CHAT SERVER SUMMARY" << std::endl;
    std::cout << "----------------------" << std::endl;
    std::cout << "Started:" << start1.tv_usec << std::endl;
    std::cout << "Duration: " << (end1.tv_sec - start1.tv_sec) << "seconds" << std::endl; 
    std::cout << "Bytes Written: " << bytesWritten << std::endl;
    std::cout << "Bytes Read: " << bytesRead << std::endl << std::endl;

    // Later : Returning to listening mode
    std::cout << "Closing Chat" << std::endl;

    return 1;
};

void Server::servListen(void){
    int totalClients = 0;

    //const int PORT = std::atoi(argv[1]);

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
    serveAddress.sin_port = htons(this->port);

    // Open TCP (stream) socket
    this->serverSd = socket(AF_INET, SOCK_STREAM, 0);

    if (this->serverSd < 0) {
        std::cerr << "Error in opening server socket" << std::endl;
        exit(0);
    }

    // BIND socket to local address
    int bindStatus = bind(this->serverSd, (struct sockaddr*) &serveAddress, sizeof(serveAddress));

    if (bindStatus < 0) {
        std::cerr << "Could not bind socket to local address" << std::endl;
        exit(0);
    }

    std::cout << "\nCHAT SERVER UP AND RUNNING!" << std::endl;

    totalClients += chatSession();
}