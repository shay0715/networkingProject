#include "user.h"
#include <string.h>
#include <sys/socket.h>

User::User(const int& socket)
{
    this->socket = socket;
    exitStatus = false;
    usernameSet = false;
}

bool User::operator==(User other)
{
    if(this->GetUsername() == other.GetUsername() && this->GetSocket() == other.GetSocket())
    {
        return true;
    }
    return false;
}

std::string User::GetUsername()
{
    return userName;
}

bool User::GetExitStatus()
{
    return exitStatus;
}

void User::SetUseranme(std::string newUserName)
{
    userName = newUserName;
    usernameSet = true;
}

bool User::getUsernameStatus()
{
    return usernameSet;
}

void User::SetExitStatus(bool status)
{
    exitStatus = status;
}

const int& User::GetSocket()
{
    return socket;
}

void User::sendMsg(std::string msg)
{
    
    const char* characterMsg = msg.c_str();
    send(socket, characterMsg, strlen(characterMsg), 0);

}

void User::sendMsgNoWhitespace(std::string msg)
{
    if(msg.find('\n') != 1)
    {
            msg = msg.substr(0, msg.find('\n'));
    }
    const char* characterMsg = msg.c_str();
    send(socket, characterMsg, strlen(characterMsg), 0);

}