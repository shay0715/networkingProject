#include <iostream>
class User
{
    public:
        User(const int& socket);

        std::string GetUsername();
        void SetUseranme(std::string);
        bool GetExitStatus();
        void SetExitStatus(bool);
        const int& GetSocket();
        void sendMsg(std::string);

    private:
        int socket;
        bool exitStatus;
        std::string userName;
};