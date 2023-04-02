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

class Server {
    private:
        int port;
        char *serverIp;
        int serverSd;

        int chatSession();
    public:
        Server(int port, int ip){
            this->port = port;
        }

        void init(void);
}