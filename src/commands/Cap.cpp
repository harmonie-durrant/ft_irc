#include "Command.hpp"

Cap::Cap(Server* server, bool auth) : Command(server, auth) {}

Cap::~Cap() {}

void Cap::execute(Client* client, std::vector<std::string> args) {
	(void) client;
	(void) args;
	// this is so that CAP LS doesn't send unknown command
}
