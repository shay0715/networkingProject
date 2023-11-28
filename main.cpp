#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include "main.h"


int main() {

    std::vector<std::thread> threadpool;
    std::vector<int> socketpool;
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(6789);

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    socklen_t adderlen = sizeof(address);
    bind(sockfd, (sockaddr*)&address, sizeof(address));

    Server server = Server();


    while(true)
    {
        int socket = 0;
        bool connected = connectToClient(socket, sockfd, address, adderlen);
        if(connected)
        {
            socketpool.push_back(socket);
            threadpool.push_back(std::thread(mainLoop, std::cref(socketpool.at(socketpool.size()-1)), std::ref(server)));
        }                
    }
    
    return 0;
}

void mainLoop(const int& socket, Server& server)
{
    User user = User(socket);

    send(socket, "Enter a username: ", strlen("Enter a username: "), 0);

    // TODO: Need loop for set user

    user.SetUseranme(getCommand(user.GetSocket()));

    while(!user.GetExitStatus())
    {

        server.InterpretCommand(getCommand(socket), user);

    }
}

bool connectToClient(int& new_socket, int& sockfd, sockaddr_in address, socklen_t adderlen )
{
    while(true)
    {
        std::cout<<"listening"<<std::endl;
        if(listen(sockfd, 100) == 0)
        {

            new_socket = accept(sockfd, (sockaddr*)&address, &adderlen);
            return true;
        }

    }
    return false;
}

std::string getCommand(const int& socket)
{
    char msg[1024] = { 0 };
    while(recv(socket, msg, 1023, 0) <= 0)
    {

    }
    return msg;
}