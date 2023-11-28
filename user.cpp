#include "user.h"
#include <string.h>
#include <sys/socket.h>

User::User(const int& socket)
{
    this->socket = socket;
    exitStatus = false;
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
    msg = '\n' + msg;
    const char* characterMsg = msg.c_str();
    send(socket, characterMsg, strlen(characterMsg), 0);

}