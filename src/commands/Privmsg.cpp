#include "Command.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

Privmsg::Privmsg(Server* server, bool auth) : Command(server, auth) {}

Privmsg::~Privmsg() {}

void Privmsg::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 3)
	{
		client->send_response(ERR_NEEDMOREPARAMS, _server, client, args[0] + " :Not enough parameters");
		return;
	}
	if (args[2][0] == ':')
		args[2].erase(0, 1);
	std::string message = args[2];
	if (args.size() > 3)
		for (size_t i = 3; i < args.size(); i++)
			message += " " + args[i];
	if (args[1][0] != '#') {
		for (std::map<int, Client*>::const_iterator it = _server->getClients().begin(); it != _server->getClients().end(); it++) {
			if (it->second->getNickname() == args[1]) {
				Client *target = it->second;
				target->send_response(-1, _server, target, ":" + client->getNickname() + " PRIVMSG " + target->getNickname() + " :" + message);
				return;
			}
		Client *target = _server->get_client_by_nick(args[1]);
		if (target != NULL) {
			target->send_response(-1, _server, target, ":" + client->getNickname() + " PRIVMSG " + target->getNickname() + " :" + message);
			return;
		}
		client->send_response(ERR_NOSUCHNICK, _server, client, args[1] + " :No such nick");
		return;
	}
	// Channel *channel = client->getChannel(args[1]);
	// if (channel == NULL)
	// {
	// 	client->send_response(ERR_NOSUCHCHANNEL, _server, client, args[1] + " :No such channel");
	// 	return;
	// }
	// if (!channel->isOnChannel(client))
	// {
	// 	client->send_response(ERR_CANNOTSENDTOCHAN, _server, client, args[1] + " :Cannot send to channel");
	// 	return;
	// }
	// channel->send_message(client, message);

	// TEMP, sends message back to sender as if they were the target
	client->send_response(-1, _server, client, ":" + client->getNickname() + " PRIVMSG " + args[1] + " :" + message);
}
