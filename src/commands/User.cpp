#include "Command.hpp"

User::User(Server* server, bool auth) : Command(server, auth) {}

User::~User() {}

void User::execute(Client* client, std::vector<std::string> args) {
	if (args.size() != 5)
	{
		client->send_response("ERR :Not enough parameters\r\n");
		return;
	}
	client->setUsername(args[1]);
	// client->setHost(args[2]);
	// client->setServerName(args[3]);
	// client->setRealname(args[4]);
}