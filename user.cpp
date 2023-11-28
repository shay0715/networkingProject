#include "user.h"

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