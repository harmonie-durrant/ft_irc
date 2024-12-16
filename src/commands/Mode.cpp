#include "Command.hpp"

Mode::Mode(Server* server, bool auth) : Command(server, auth) {}

Mode::~Mode() {}

void Mode::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response(ERR_NEEDMOREPARAMS, _server, client, args[0] + " :Not enough parameters");
		return;
	}
	std::string nick = client->getNickname();
	client->send_response(-1, _server, client, ":" + nick + " MODE " + nick + " " +args[2]);
}
