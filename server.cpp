#include <iostream>
#include <stdio.h>
#include <string>

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

    return 0;
}