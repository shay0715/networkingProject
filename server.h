#include <iostream>
#include <vector>
#include "group.h"


int nextPort();

class Server
{
    public:
        Server();
        Server(int);
        void InterpretCommand(std::string, std::string);
    private:
        int portNumber;
        std::vector<Group> groupList;
    
};