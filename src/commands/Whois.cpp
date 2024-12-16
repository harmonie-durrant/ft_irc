#include "Command.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

Whois::Whois(Server* server, bool auth) : Command(server, auth) {}

Whois::~Whois() {}

void Whois::execute(Client* client, std::vector<std::string> args) {
	std::string servername = "localhost";
	if (args.size() < 2)
	{
		client->send_response(ERR_NONICKNAMEGIVEN, _server, client, "No nickname given");
		return;
	}
	if (args.size() > 2)
	{
		client->send_response(ERR_NOORIGIN, _server, client, "Too many targets");
		return;
	}
	for (std::map<int, Client*>::iterator it = _server->getClients().begin(); it != _server->getClients().end(); it++)
	{
		//! if user is hidden from WHOIS, break
		if (it->second->getNickname() == args[1])
		{
			Client *target = it->second;
			std::string nick = target->getNickname();
			std::string fullname = target->getFullname();
			std::string hostname = target->getHostname();
			client->send_response(RPL_WHOISUSER, _server, client, nick + " " +fullname + " " + hostname);
			client->send_response(RPL_WHOISSERVER, _server, client, servername);
			client->send_response(RPL_ENDOFWHOIS, _server, client, "End of WHOIS list.");
			return;
		}
	}
	//! check if input is a channel
	//! for loop on server channels
    client->send_response(ERR_NOSUCHNICK, _server, client, args[1] + " :No such nick/channel");
}
