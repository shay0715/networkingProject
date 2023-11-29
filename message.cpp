#include "message.h"
#include <algorithm>
#include "UI.h"

std::string maxString(std::string, std::string);

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
    int endLineIndex = postDate.find('\n');
    if(endLineIndex >= 0)
    {
        postDate = postDate.substr(0,endLineIndex);
    }
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
    std::string msgIDString = "| MESSAGE ID: " + msgID + " |";
    std::string senderString = "| SENDER: " + sender + " |";
    std::string postDateString = "| POST DATE: " + postDate + " |";
    std::string subjectString = "| SUBJECT: " + subject + " |";

    int postSize = maxString(maxString(msgIDString, senderString), maxString(postDateString, subjectString)).size() + 5;

    std::string hyphens = UI::HyphenGenerator(postSize) +'\n';

    msgIDString += UI::HyphenGenerator(postSize - msgIDString.size()-1) + "|\n";
    senderString += UI::HyphenGenerator(postSize - senderString.size()-1) + "|\n";
    postDateString += UI::HyphenGenerator(postSize - postDateString.size()-1) + "|\n";
    subjectString += UI::HyphenGenerator(postSize - subjectString.size()-1) + "|\n";

    std::string returnMsg = hyphens + msgIDString + senderString + postDateString + subjectString + hyphens;
    return returnMsg;
}

std::string maxString(std::string firstString, std::string secondString)
{

    if(firstString.size() >= secondString.size())
    {
        return firstString;
    }
    else
    {
        return secondString;
    }
}