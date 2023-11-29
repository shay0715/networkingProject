#include "group.h"
#include <algorithm>
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
    this->groupID = groupID;
    msgID = 0;
}

bool Group::AddToUserList(User& newUser)
{
    for(User user : userList)
    {
        if(user.GetUsername() == newUser.GetUsername())
        {
            newUser.sendMsg("That username has already been taking in this bulletin board\n");
            return false;
        }
    }

    userList.emplace_back(newUser);
    std::string msg = "SERVER MESSAGE: Welcome " + newUser.GetUsername() + " to the bulletin-board: " + groupName + '\n';
    sendMsgAll(msg);
    PrintUserListAll(newUser);

    return true;
}

void Group::PrintUserList(User user)
{
    user.sendMsg(" - Current Active Users: " + std::to_string(userList.size()) + " -\n");

    for(User userFromList : userList)
    {
        user.sendMsg(" +-"+userFromList.GetUsername()+'\n');
    }
}

void Group::PrintUserListAll(User user)
{
    sendMsgAll(" - Current Active Users: " + std::to_string(userList.size()) + " -\n");
    for(User user: userList)
    {
        sendMsgAll(" +-"+user.GetUsername()+'\n');
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

void Group::InterpretCommand(std::string command, User& user)
{
    // command needs to equal 0 and not be greater than 0 to ensure a command isn't inside a post

    if(command.find("%join") == 0)
    {
        bool joined = Join(user);
        if(joined)
        {
            DisplayRecentMsgs(user);
        }
        else
        {
            user.sendMsg("Count not be joined\n");
        }
    }
    
    else if(command.find("%post") == 0 && isInUserList(user))
    {
        std::vector<std::string> argVector = parseString(command);
        Post(std::move(argVector.at(0)), std::move(argVector.at(1)), user);
    }

    else if(command.find("%users")== 0 && isInUserList(user))
    {
        PrintUserList(user);
    }

    else if(command.find("%leave") == 0 && isInUserList(user))
    {
        Leave(user);
    }

    else if(command.find("%message") == 0 && isInUserList(user))
    {
        std::vector<std::string> argVector = parseString(command);
        GetMessage(std::move(argVector.at(0)), user);
    }

    else if(command.find("%exit") == 0 && isInUserList(user))
    {
        Exit(user);
    }
    else
    {
        if(!isInUserList(user))
        {
            user.sendMsg("!! You have not joined " + groupName + " yet !!\n");
            user.sendMsg("!! Join " + groupName + " with the command \"%gropujoin -" + groupName + "\" or \"%groupjoin -" + std::to_string(groupID) + "\" !!\n");
        }
        else
        {
            user.sendMsg("!! There was a problem with your command !!\n!! Try using \"%help\" if you're confused !!\n");
        }
    }
}

bool Group::Join(User user)
{
    AddToUserList(user);
    return true;
}

bool Group::Post(std::string subject, std::string body, User& user)
{
    std::string msgID = std::to_string(GetNextMsgID());
    Message msg = Message(subject, body, user.GetUsername(), msgID);
    msgList.push_back(msg);
    sendMsgAll("\t--Room: " + groupName +"--\n");
    sendMsgAll(msg.PrintMsg()+'\n');
    return true;
}

bool Group::Leave(User user)
{
    RemoveFromUserList(user.GetUsername());
    user.sendMsg("Leaving " + groupName + '\n');
    return true;
}

bool Group::GetMessage(std::string msgID, User user)
{
    for(Message msg : msgList)
    {
        if(msgID == msg.GetMsgID())
        {
            std::string msgContent = "-- Getting message inside post --\n";
            msgContent += " -+ Message: " +msg.GetBody() + " +-\n";
            user.sendMsg(msgContent);
            user.sendMsg("\n");
        }
    }
    return true;
}

bool Group::Exit(User& user)
{
    Leave(user);
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
        user.sendMsg("\t\t --| Recent Post(s) |--\n");
        user.sendMsg(msgList.at(msgList.size()-2).PrintMsg() + '\n');
        user.sendMsg(msgList.at(msgList.size()-1).PrintMsg() + '\n');
    }
    else if (msgList.size() == 1)
    {
        user.sendMsg("\t\t --| Recent Post(s) |--\n");
        user.sendMsg(msgList.at(msgList.size()-1).PrintMsg() + '\n');
    }
    else
    {
        user.sendMsg("There are no post yet! You can be the first!\n");
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

void Group::sendMsgAll(std::string msg)
{
    for(User user : userList)
    {
        user.sendMsg(msg);
    }
}

bool Group::isInUserList(User user)
{
    return (std::find(userList.begin(), userList.end(), user) != userList.end());
}