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
	std::string message = args[2];
	for (size_t i = 3; i < args.size(); i++)
		message += " " + args[i];
	if (args[1][0] != '#') {
		for (std::map<int, Client*>::iterator it = _server->getClients().begin(); it != _server->getClients().end(); it++) {
			if (it->second->getNickname() == args[1]) {
				Client *target = it->second;
				target->send_response(-1, _server, target, ":" + client->getNickname() + " PRIVMSG " + args[1] + " :" + message);
				return;
			}
		}
		client->send_response(401, _server, client, args[1] + " :No such nick/channel");
		return;
	}
	//! for loop to send to every channel
	// client->send_response(401, _server, client, args[1] + " :No such nick/channel");
	
	// DEBUG, sends message back to sender as if they were the target
	client->send_response(-1, _server, client, ":" + client->getNickname() + " PRIVMSG " + args[1] + " :" + message);
}
