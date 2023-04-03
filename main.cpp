// Simple server - client chat using TCP and Linux
#include <new>
// Server and Client classes can be found in the object_oriented_version folder
#include "./object_oriented_version/server/server.h"
#include "./object_oriented_version/client/client.h"

// Creates an instance of Server and runs servListen()
void createServer(void);
// Creates an instance of Client and runs clientConnect()
void createClient(void);


int main(){
    std::cout << "WELCOME TO THE SERVER - CLIENT CHAT APPLICATION" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl << std::endl;

    // Asking the user whether they want to make a server or client
    int selection = -1;
    while (selection != 1 && selection != 2) {
        std::cout << "Write 1 to create a server; write 2 to create a client" << std::endl;
        std::cin >> selection;

        if (selection != 1 && selection != 2) {
            std::cout << std::endl << selection << " is not valid.\n\n" << std::endl;
        }
    }

    if (selection == 1){
        createServer();
    } else {
        createClient();
    }

    std::string userSelected = (selection == 1) ? "server" : "client";
    std::cout << "\nThanks for trying out our simple Linux chat " << userSelected << "!" << std::endl;
    std::cout << "Have a wonderful day!" << std::endl;

    return 0;
}

// getPort() Used by createServer and createClient to get an unregistered port from the user
long getPort(){
    long port = -1;

    // Useable ports: 49152 to 65535
    while (port < 49152 || port > 65535){
        std::cout << "Please enter a port number between 49152 and 65535 (inclusive):" << std::endl;
        std::cin >> port;
        
        if (port < 49152 || port > 65535) {
            std::cout << std::endl << port << "is an invalid selection" << std::endl;
        }    
    }
    
    return port;
}

// Creates an instance of server and runs chat from server end
void createServer(void){
    long port = getPort();

    Server *server = new Server(port);
    server->servListen();
}

// Creates an instance of client and runs chat from client end
void createClient(void){
    long port = getPort();
    char ip[15];

    // Flushing newline character
    std::cin.ignore();
    std::cout << "Please enter a valid IP in dotted decimal format (xxx.xxx.xxx.xxx)" << std::endl;

    // Max IP = "xxx.xxx.xxx.xxx"
    std::cin.getline(ip, 15);

    Client *client = new Client(port, ip);
    client->clientConnect();
}