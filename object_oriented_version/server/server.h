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
        long port;
        int serverSd;

        // used by servListen; an individual chat with a client
        int chatSession(void);
    public:
        Server(long port){
            this->port = port;
        }

        // Runs chat from the server end
        // (Later: Will be able to continue to run and keep track of clients joined)
        void servListen(void);
};