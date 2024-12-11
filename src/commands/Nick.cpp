#include "Command.hpp"

Nick::Nick(Server* server, bool auth) : Command(server, auth) {}

Nick::~Nick() {}

void Nick::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response(461, _server, client, args[0] + " :Not enough parameters");
		return;
	}
	client->setNickname(args[1]);
	// client->send_response(-1, _server, client, ":" + args[1] + " NICK " + args[1];
}