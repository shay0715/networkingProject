#include "group.h"

bool createUsername(Group&);
bool connectToClient(int&, int&, sockaddr_in, socklen_t);
std::string getCommand(const int&);
void mainLoop(const int&, Group&);