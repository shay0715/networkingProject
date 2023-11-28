#include "group.h"
#include <sys/socket.h>
#include <cstring>
#include <string.h>

Group::Group()
{
    groupName = "default";
    msgID = 0;
};

Group::Group(std::string groupName, int groupID)
{
    this->socket = socket;
    this->groupName = groupName;
    this->groupID;
    msgID = 0;
}

bool Group::AddToUserList(User newUser)
{
    for(User user : userList)
    {
        if(user.GetUsername() == newUser.GetUsername())
        {
            sendMsg("That username has already been taking in this bulletin board", newUser);
            return false;
        }
    }

    userList.push_back(newUser);
    std::string msg = "SERVER MESSAGE: Welcome " + newUser.GetUsername() + " to the bulletin-board: " + groupName;
    sendMsgAll(msg);
    PrintUserListAll();

    return true;
}

void Group::PrintUserList(User user)
{
    for(User user : userList)
    {
        sendMsg(user.GetUsername(), user);
    }
}

void Group::PrintUserListAll()
{
    for(User user: userList)
    {
        sendMsgAll(user.GetUsername());
    }
}

std::vector<User> Group::GetUserList()
{
    return userList;
}

std::string Group::GetCurrentUser()
{
    return currentUser;
}

bool Group::RemoveFromUserList(std::string username)
{
    for(int i=0; i < userList.size(); i++)
    {
        if(username == userList.at(i).GetUsername())
        {
            userList.erase(userList.begin() + i);
            return true;
        }
    }
    return false;
}

void Group::InterpretCommand(std::string command, User user)
{
    // command needs to equal 0 and not be greater than 0 to ensure a command isn't inside a post
    if(command.find("%connect") == 0)
    {
        std::vector<std::string> argVector = parseString(command);
        if(!argVector.empty())
        {
            Connect(std::move(argVector.at(0)), std::move(argVector.at(1)));
        }

        if(!user.GetExitStatus())
        {
            sendMsg("Connected successfully", user);
        }
        else
        {
            sendMsg("Could not be connected", user);
        }
    }

    else if(command.find("%join") == 0 && !user.GetExitStatus())
    {
        Join();

        if(joinedStatus)
        {
            sendMsg("Joined successfully", user);
            DisplayRecentMsgs(user);
        }
        else
        {
            sendMsg("Count not be joined", user);
        }
    }
    
    else if(command.find("%post") == 0 && joinedStatus)
    {
        std::vector<std::string> argVector = parseString(command);
        Post(std::move(argVector.at(0)), std::move(argVector.at(1)), user);
    }

    else if(command.find("%users") == 0 && joinedStatus)
    {
        PrintUserList(user);
    }

    else if(command.find("%leave") == 0 && joinedStatus)
    {
        Leave();
    }

    else if(command.find("%message") == 0 && joinedStatus)
    {
        std::vector<std::string> argVector = parseString(command);
        GetMessage(std::move(argVector.at(0)), user);
    }

    else if(command.find("%exit") == 0)
    {
        Exit(user);
    }

    else if(command.find("%help") == 0)
    {
        if(command == "%help" && joinedStatus)
        {
            sendMsg("List of all commands: ", user);
            for(std::string exampleCommand : commandList)
            {
                sendMsg(exampleCommand, user);
            }

            sendMsg("Type \"%help -[command]\" for help with using a specific command", user);
        }
        else if(command.find("%help") && joinedStatus)
        {
            std::vector<std::string> argVector = parseString(command);
            // Write help function
            std::string msg = "You want help with " + argVector.at(0) + "?"; 
            sendMsg(msg, user);
        }
        else
        {
            std::string msg = "Use %connect -[ADDRESS] -[PORT] to connect to a server \n Use %join to join a bulliten board";
            sendMsg(msg, user);
        }
    }
    
    else
    {
        if(joinedStatus)
        {
            std::string msg = "The command you entered doesn't exist. Make sure you began your command with % and you don't have any leading whitespace. Use %help to see the list of commands.";
            sendMsg(msg, user);
        }
        else
        {
            std::string msg = "The command you entered doesn't exist or cannot be used until you connect to a server and join a bulliten board. Make sure you began your command with % and you don't have any leading whitespace. Use %help to see what commands you have access to.";
            sendMsg(msg, user);
        }
    }

}

bool Group::Connect(std::string address, std::string port)
{
    return true;
}


bool Group::Join()
{
    joinedStatus = true;
    return true;
}

bool Group::Post(std::string subject, std::string body, User user)
{
    std::string msgID = std::to_string(GetNextMsgID());
    Message msg = Message(subject, body, user.GetUsername(), msgID);
    msgList.push_back(msg);
    sendMsgAll(msg.PrintMsg());
    return true;
}

bool Group::Leave()
{
    joinedStatus = false;
    return true;
}

bool Group::GetMessage(std::string msgID, User user)
{
    for(Message msg : msgList)
    {
        if(msgID == msg.GetMsgID())
        {
            std::string msgContent = "The body of message " + msgID + " is: " + msg.GetBody();
            sendMsg(msgContent, user);
        }
    }
    return true;
}

bool Group::Exit(User user)
{
    if(joinedStatus == true)
    {
        sendMsg("Please use %leave to leave the bulliten board first", user);
        return false;
    }

    user.SetExitStatus(true);
    return true;
}

int Group::GetCurrentMsgID()
{
    return msgID;
}

int Group::GetNextMsgID()
{
    msgID += 1;
    return msgID;
}

bool Group::GetJoinedStatus()
{
    return joinedStatus;
}

int Group::GetGroupID()
{
    return groupID;
}

std::string Group::GetGroupName()
{
    return groupName;
}

void Group::DisplayRecentMsgs(User user)
{
    if(msgList.size() >= 2)
    {
        sendMsg(msgList.at(msgList.size()-2).PrintMsg(), user);
        sendMsg(msgList.at(msgList.size()-1).PrintMsg(), user);
    }
    else if (msgList.size() == 1)
    {
        sendMsg(msgList.at(msgList.size()-1).PrintMsg(), user);
    }
    else
    {
        sendMsg("There are no post yet! You can be the first!", user);
    }
}

std::vector<std::string> parseString(std::string stringToParse)
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

void Group::sendMsg(std::string msg, User user)
{
    msg = '\n' + msg;
    const char* characterMsg = msg.c_str();
    send(user.GetSocket(), characterMsg, strlen(characterMsg), 0);

}

void Group::sendMsgAll(std::string msg)
{
    for(User user : userList)
    {
        sendMsg(msg, user);
    }
}