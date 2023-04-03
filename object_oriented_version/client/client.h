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

class Client {
    private:
        long port;
        char *serverIp;
    public:
        Client(long port, char* serverIp) {
            this->port = port;
            this->serverIp = serverIp;
        }

        // Runs chat from the client end
        void clientConnect(void);
};