#include <iostream>
#include <ctime>

class Message
{
    public:
        Message();
        Message(std::string, std::string, std::string, std::string);
        std::string GetMsgID();
        std::string GetBody();
        std::string PrintMsg();
    private:
        std::string msgID;
        std::string sender;
        std::string postDate;
        std::string subject;
        std::string body;
};