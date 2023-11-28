#include "server.h"


bool connectToClient(int&, int&, sockaddr_in, socklen_t);
std::string getCommand(const int&);
void mainLoop(const int&, Server&);