#include <iostream>
class User
{
    public:
        User(const int& socket);

        bool operator==(User);
        std::string GetUsername();
        void SetUseranme(std::string);
        bool GetExitStatus();
        void SetExitStatus(bool);
        const int& GetSocket();
        void sendMsg(std::string);
        bool getUsernameStatus();
        void sendMsgNoWhitespace(std::string);

    private:
        int socket;
        bool exitStatus;
        bool usernameSet;
        std::string userName;
};