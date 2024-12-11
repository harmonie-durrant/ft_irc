#include "Command.hpp"

Privmsg::Privmsg(Server* server, bool auth) : Command(server, auth) {}

Privmsg::~Privmsg() {}

void Privmsg::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 3)
	{
		client->send_response(461, _server, client, args[0] + " :Not enough parameters");
		return;
	}
	if (args[2][0] == ':')
		args[2].erase(0, 1);
	//! check if args[1] is a channel or a user
	//! send to all users in channel or the target user
	client->send_response(-1, _server, client, ":" + client->getNickname() + " PRIVMSG " + args[1] + " :" + args[2]);
}