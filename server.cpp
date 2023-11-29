#include "server.h"
#include "UI.h"
#include <sys/socket.h>
#include <string.h>

Server::Server()
{
    std::vector<std::string> roomNames = {"Dogs" , "Cats", "Programming", "Misc.", "Coffee"};
    for(int i=0; i<5; i++)
    {
        std::string groupName = roomNames.at(i);
        Group group = Group(groupName, i+1);
        groupList.emplace_back(std::move(group));
    }
}

void Server::InterpretCommand(std::string command, User& user)
{
    //TODO: write getgroups
    if(command == "%groups")
    {
        ListGroups(user);
    }
    else if(command.find("%help") == 0)
    {
        if(command == "%help")
        {
            user.sendMsg("== The following is a list of all commands ==\n");
            for(std::string exampleCommand : commandList)
            {
                user.sendMsg("+= "+ exampleCommand + '\n');
            } 

            user.sendMsg("== Use \"%help -[command]\" for help with using a specific command ==\n");
        }
        else
        {
            std::vector<std::string> argVector = parseString(command);
            std::string arg = argVector.at(0);

            std::string msg = "";

            if(arg == "%group")
            {
                msg = "== \"%group\" list all active groups and takes no arguments ==\n";
            }
            else if(arg == "%groupjoin")
            {
                msg = "== \"%groupjoin -[ROOMNAME | ROOMID]\" connects a user to a room. takes one required argument as room name\\room ID ==\n";
                msg += "== Example Usage: \"%groupjoin -Dogs\" | \"%groupjoin -1\" ==\n";
            }
            else if(arg == "%grouppost")
            {
                msg = "== \"%grouppost -[ROOMNAME | ROOMID] -[SUBJECT] -[BODY]\" creates a post on a specified room. takes three required arugments as room name\\room ID, post subject, post body == \n";
                msg += "== Example Usage: \"%grouppost -1 -Subject -Body\" ==\n";
            }
            else if(arg == "%groupusers")
            {
                msg = "== \"%groupusers -[ROOMNAME | ROOMID]\" prints all users in a room. Takes one required arguement as room name \\ room ID ==\n";
                msg += "== Example Usage: \"%groupusers -1\" ==\n";
            }
            else if(arg == "%groupleave")
            {
                msg = "== \"%groupleave -[ROOMNAME | ROOMID]\" disconnects a user from a room. Takes one required argument as room name \\ room ID ==\n";
                msg += "== Example Usage: \"%groupleave -1\" ==\n";
            }
            else if(arg =="%groupmessage")
            {
                msg = "== \"%groupmessage -[ROOMNAME | ROOMID] -[MSGID]\" retrieves and prints the body of a post. Takes two required arguments as the room name \\ room ID, post ID ==\n";
                msg += "== Example Usage: \"%groupmessage -1 -1\" ==\n";
            }
            else if(arg == "%exit")
            {
                msg = "== \"%exit\" disconnects a user from all rooms and the server. Takes no arguments ==\n";
            }
            
            user.sendMsg(msg);
        }
    }
    else if(command == "%exit")
    {
        for(int i=0; i<groupList.size(); i++)
        {
            for(int j=0; j<groupList.at(i).GetUserList().size(); j++)
            {
                if(user.GetUsername() == groupList.at(i).GetUserList().at(j).GetUsername())
                {
                    groupList.at(i).Exit(user);
                }
            }
        }
        user.SetExitStatus(true);
    }
    else
    {
        std::vector<std::string> argVector = parseString(command);
        int id = -1;

        if(!argVector.empty())
        {
            char firstSymbol = argVector.at(0)[0];


            if(isdigit(firstSymbol))
            {
                id = std::stoi(argVector.at(0)); 
            }

            for(int i =0; i < groupList.size(); i++)
            {
                if(id > 0)
                {
                    if(groupList.at(i).GetGroupID() == id)
                    {
                        int groupSubstrIndex = command.find("group");
                        command = '%' + command.substr(groupSubstrIndex + 5);
                        command = GetGroupCommand(command, argVector);

                        groupList.at(i).InterpretCommand(command, user);
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

                        groupList.at(i).InterpretCommand(command, user);
                        return;
                    }
                }
            }
        std::cout << "No group with that name or ID exist" << std::endl;
        }
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

void Server::ListGroups(User user)
{
    std::string msg = "  |------Currently Active Rooms------|\n";
    int hyphenAmt = msg.size();
    std::string hypensForMsg = UI::HyphenGenerator(hyphenAmt-5);
    msg += "  |" + hypensForMsg + "|\n";
    for(Group group : groupList)
    {
        std::string msgToAdd = "  |--Room \"" + group.GetGroupName() + "\"" + "--ID " + std::to_string(group.GetGroupID());
        msgToAdd += UI::HyphenGenerator(hyphenAmt-msgToAdd.size() - 2)+"|\n";

        msg += "  |"+UI::HyphenGenerator(hyphenAmt -5 ) + "|\n";
        msg += msgToAdd;
        msg += "  |" + UI::HyphenGenerator(hyphenAmt -5 ) + "|\n";
    }
    user.sendMsg(msg);
}