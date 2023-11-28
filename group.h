#include <iostream>
#include <vector>
#include "message.h"
#include "user.h"

std::vector<std::string> parseString(std::string);
// Represents a single bulletin-board group
class Group 
{
    public:
        
        int socket;


        // Constructors
        Group();
        Group(std::string, int);
        
        // Methods for list of users
        void PrintUserList(User user);
        void PrintUserListAll();
        bool AddToUserList(User&);
        bool RemoveFromUserList(std::string);
        std::vector<User> GetUserList();
        std::string GetCurrentUser();

        // Methods for commands
        Group InterpretCommand(std::string, User& user);
        bool Connect(std::string, std::string);
        bool Join(User user);
        bool Post(std::string, std::string, User& user);
        bool Leave(User user);
        bool GetMessage(std::string, User user);
        bool Exit(User user);

        // Methods for messages
        int GetCurrentMsgID();
        int GetNextMsgID();
        void sendMsgAll(std::string);
        void DisplayRecentMsgs(User user);


        // Get Statuses
        int GetGroupID();
        std::string GetGroupName();

    private:
        std::vector<User> userList;
        std::string groupName;
        int groupID;
        int msgID;
        std::vector<Message> msgList;
        std::string currentUser;
        std::vector<std::string> commandList = {"%connect", "%join", "%post", "%users", "%leave", "%messages", "%exit"};

};