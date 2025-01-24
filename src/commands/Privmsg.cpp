#include "Command.hpp"
#include "Server.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

Privmsg::Privmsg(Server* server, bool auth) : Command(server, auth) {}

Privmsg::~Privmsg() {}

void Privmsg::execute(Client* client, std::vector<std::string> args)
{
	if (args.size() < 3)
	{
		client->send_response(ERR_NEEDMOREPARAMS, client, args[0] + " :Not enough parameters");
		return;
	}
	if (args[2][0] == ':')
		args[2].erase(0, 1);
	std::string message = args[2];
	if (args.size() > 3)
		for (size_t i = 3; i < args.size(); i++)
			message += " " + args[i];
	if (args[1][0] != '#')
	{
		std::map<int, Client *> clients = _server->getClients();
		for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			if (it->second->getNickname() == args[1])
			{
				Client *target = it->second;
				target->send_response(-1, target, ":" + client->getNickname() + " PRIVMSG " + target->getNickname() + " :" + message);
				return;
			}
		}
		Client *target = _server->get_client_by_nick(args[1]);
		if (target != NULL)
		{
			target->send_response(-1, target, ":" + client->getNickname() + " PRIVMSG " + target->getNickname() + " :" + message);
			return;
		}
		client->send_response(ERR_NOSUCHNICK, client, args[1] + " :No such nick");
		return;
	}
	Channel *channel = _server->getChannel(args[1]);
	if (channel == NULL)
		return client->send_response(ERR_NOSUCHCHANNEL, client, args[1] + " :No such channel");
	std::vector<Client *> clients = channel->getClients();
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
		if (client->getNickname() == (*it)->getNickname()) {
			for (std::vector<Client *>::iterator iter = clients.begin(); iter != clients.end(); iter++) {
				if (client->getNickname() == (*iter)->getNickname())
					continue;
				return (*iter)->send_response(-1, *iter, ":" + client->getNickname() + " PRIVMSG " + channel->getName() + " :" + message);
			}
		}
	}
	return client->send_response(ERR_CANNOTSENDTOCHAN, client, args[1] + " :Cannot send to channel");
}
