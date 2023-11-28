#include "message.h"

Message::Message()
{

}

Message::Message(std::string subject, std::string body, std::string sender, std::string msgID)
{
    this->subject = subject;
    this->body = body;
    this->sender = sender;
    this->msgID = msgID;
    const time_t currentTime = time(0);
    this->postDate = ctime(&currentTime);
}

std::string Message::GetMsgID()
{
    return msgID;
}

std::string Message::GetBody()
{
    return body;
}

std::string Message::PrintMsg()
{
    return "MESSAGE ID: " + msgID + " SENDER: " + sender
    + " POST DATE: " + postDate + " SUBJECT: " + subject;
}