#include "Command.hpp"

Privmsg::Privmsg(Server* server, bool auth) : Command(server, auth) {}

Privmsg::~Privmsg() {}

void Privmsg::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response("ERR :Not enough parameters\r\n");
		return;
	}
	client->send_response(":" + client->getNickname() + " PRIVMSG " + args[1] + " :" + args[2] + "\r\n");
}