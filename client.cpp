#include <iostream>
#include <stdio.h>
#include <string>

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

    

    return 0;
}