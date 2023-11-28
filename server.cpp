#include "server.h"
#include <sys/socket.h>
#include <string.h>

Server::Server()
{
    for(int i=0; i<5; i++)
    {
        std::string groupName = "Group: " + std::to_string(i + 1);
        Group group = Group(groupName, i+1);
        std::cout << groupName << std::endl;
        groupList.emplace_back(std::move(group));
    }
}

Server::Server(int amountOfGroups)
{
    for(int i=0; i<amountOfGroups; i++)
    {
        std::string groupName = "Group: " + std::to_string(i+1);
        Group group = Group(groupName, i+1);
        groupList.emplace_back(std::move(group));
    }
}

void Server::InterpretCommand(std::string command, User user)
{
    //TODO: write getgroups
    if(command == "%groups")
    {
        for(Group group : groupList)
        {
            std::string msg = "The names of groups are: " + group.GetGroupName() + " the id's of the groups are: " + std::to_string(group.GetGroupID());
            sendMsg(msg, user);
        }
    }
    else if(command == "%help")
    {

    }
    else
    {
        std::vector<std::string> argVector = parseString(command);
        int id = -1;

        if(!argVector.empty())
        {
            char firstSymbol = argVector.at(0)[0];

            std::cout << firstSymbol << std::endl;

            if(isdigit(firstSymbol))
            {
                id = std::stoi(argVector.at(0)); 
                std::cout << id << std::endl;
            }
        }

        for(int i =0; i < groupList.size(); i++)
        {
            //TODO fix this for loop with Group& group : groupList
            if(id > 0)
            {
                if(groupList.at(i).GetGroupID() == id)
                {
                    int groupSubstrIndex = command.find("group");
                    command = '%' + command.substr(groupSubstrIndex + 5);
                    command = GetGroupCommand(command, argVector);

                    std::cout << command << std::endl;
                    groupList.at(i) = std::move(groupList.at(i).InterpretCommand(command, user));
                    groupList.at(i).PrintUserList(user);
                    return;
                }
            }
            else
            {
                if(groupList.at(i).GetGroupName() == argVector.at(0))
                {
                     int groupSubstrIndex = command.find("group");
                    command = '%' + command.substr(groupSubstrIndex + 5);

                    command = GetGroupCommand(command, argVector);
                                        
                    std::cout<< command << std::endl;

                    groupList.at(i).InterpretCommand(command, user);
                    return;
                }
            }
        }
        std::cout << "No group with that name or ID exist" << std::endl;
    }
}

std::vector<std::string> Server::parseString(std::string stringToParse)
{
    std::vector<std::string> argVector;
    int argPos = stringToParse.rfind('-');
    while(argPos > 0)
    {
        if(argVector.empty())
        {
            argVector.push_back(stringToParse.substr(argPos+1));
        }
        else
        {
            argVector.insert(argVector.begin(), stringToParse.substr(argPos+1));

        }

        stringToParse = stringToParse.substr(0, argPos);
        argPos = stringToParse.rfind('-');
    }
    return std::move(argVector);
} 

std::string Server::GetGroupCommand(std::string command, std::vector<std::string> argVector)
{
    int argPos = command.find('-');
    std::string commandToReturn = command.substr(0, argPos);

    argVector.erase(argVector.begin());

    for(std::string arg : argVector)
    {
        commandToReturn += '-' + arg;
    }

    return commandToReturn;
}

void Server::sendMsg(std::string msg, User user)
{
    msg = '\n' + msg;
    const char* characterMsg = msg.c_str();
    send(user.GetSocket(), characterMsg, strlen(characterMsg), 0);

}