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
    this->groupID = groupID;
    msgID = 0;
}

bool Group::AddToUserList(User& newUser)
{
    for(User user : userList)
    {
        if(user.GetUsername() == newUser.GetUsername())
        {
            newUser.sendMsg("That username has already been taking in this bulletin board");
            return false;
        }
    }

    userList.emplace_back(newUser);
    std::string msg = "SERVER MESSAGE: Welcome " + newUser.GetUsername() + " to the bulletin-board: " + groupName;
    sendMsgAll(msg);
    PrintUserListAll();

    return true;
}

void Group::PrintUserList(User user)
{
    for(User user : userList)
    {
        user.sendMsg(user.GetUsername());
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

Group Group::InterpretCommand(std::string command, User& user)
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
            user.sendMsg("Connected successfully");
        }
        else
        {
            user.sendMsg("Could not be connected");
        }
    }

    else if(command.find("%join") == 0)
    {
        bool joined = Join(user);
        if(joined)
        {
            user.sendMsg("Joined successfully");
            DisplayRecentMsgs(user);
        }
        else
        {
            user.sendMsg("Count not be joined");
        }
    }
    
    else if(command.find("%post") == 0)
    {
        std::cout<<"posting"<<std::endl;
        std::vector<std::string> argVector = parseString(command);
        Post(std::move(argVector.at(0)), std::move(argVector.at(1)), user);
    }

    else if(command.find("%users") == 0)
    {
        PrintUserList(user);
    }

    else if(command.find("%leave") == 0)
    {
        Leave();
    }

    else if(command.find("%message") == 0)
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
        if(command == "%help")
        {
            user.sendMsg("List of all commands: ");
            for(std::string exampleCommand : commandList)
            {
                user.sendMsg(exampleCommand);
            }

            user.sendMsg("Type \"%help -[command]\" for help with using a specific command");
        }
        else
        {
            std::vector<std::string> argVector = parseString(command);
            // Write help function
            std::string msg = "You want help with " + argVector.at(0) + "?"; 
            user.sendMsg(msg);
        }
    }
    return *this;
}

bool Group::Connect(std::string address, std::string port)
{
    return true;
}


bool Group::Join(User user)
{
    std::cout<<"joined"<<std::endl;
    AddToUserList(user);
    return true;
}

bool Group::Post(std::string subject, std::string body, User& user)
{
    std::cout<<"in post"<<std::endl;
    std::string msgID = std::to_string(GetNextMsgID());
    Message msg = Message(subject, body, user.GetUsername(), msgID);
    msgList.push_back(msg);
    std::cout<<msg.PrintMsg()<<std::endl;
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
            user.sendMsg(msgContent);
        }
    }
    return true;
}

bool Group::Exit(User user)
{
    if(joinedStatus == true)
    {
        user.sendMsg("Please use %leave to leave the bulliten board first");
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
        user.sendMsg(msgList.at(msgList.size()-2).PrintMsg());
        user.sendMsg(msgList.at(msgList.size()-1).PrintMsg());
    }
    else if (msgList.size() == 1)
    {
        user.sendMsg(msgList.at(msgList.size()-1).PrintMsg());
    }
    else
    {
        user.sendMsg("There are no post yet! You can be the first!");
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
    std::cout<<"sending"<<std::endl;
    for(User user : userList)
    {
        std::cout << user.GetUsername() << std::endl;
        user.sendMsg(msg);
    }
}