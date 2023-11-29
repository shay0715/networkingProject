#include <iostream>
#include <vector>
#include "group.h"


class Server
{
    public:
        Server();
        Server(int);
        void InterpretCommand( std::string, User& user);
        void AddUserToGroup();
        std::vector<std::string> parseString(std::string);
        std::string GetGroupCommand(std::string, std::vector<std::string>);
        void sendMsg(std::string msg, User user);
        void ListGroups(User);
    private:

        std::vector<std::string> commandList = {"%groups", "%groupjoin", "%grouppost", "%groupusers", "%groupleave", "%groupmessages", "%exit"};

        std::vector<Group> groupList;
    
};