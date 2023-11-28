#include "server.h"

Server::Server()
{
    for(int i=0; i<5; i++)
    {
        std::string groupName = "Group: " + i;
        Group group = Group(groupName, i);
        groupList.emplace_back(std::move(group));
    }
    portNumber = 8000;
}

Server::Server(int amountOfGroups)
{
    for(int i=0; i<amountOfGroups; i++)
    {
        std::string groupName = "Group: " + i;
        Group group = Group(groupName, i);
        groupList.emplace_back(std::move(group));
    }
}

void Server::InterpretCommand(std::string groupID, std::string command)
{
    char firstSymbol = groupID[0];
    int id = -1;

    if(isdigit(firstSymbol))
    {
        int id = std::stoi(groupID); 
    }

    for(Group group : groupList)
    {
        if(id > 0)
        {
            if(group.GetGroupID() == id)
            {
                //group.InterpretCommand(command);
                return;
            }
        }
        else
        {
            if(group.GetGroupName() == groupID)
            {
                //group.InterpretCommand(command);
                return;
            }
        }
    }
    std::cout << "No group with that name or ID exist" << std::endl;
}
