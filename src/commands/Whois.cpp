#include "Command.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

Whois::Whois(Server* server, bool auth) : Command(server, auth) {}

Whois::~Whois() {}

void Whois::execute(Client* client, std::vector<std::string> args) {
	std::string servername = "localhost";
	if (args.size() < 2)
	{
		client->send_response(ERR_NONICKNAMEGIVEN, client, "No nickname given");
		return;
	}
	if (args.size() > 2)
	{
		client->send_response(ERR_NOORIGIN, client, "Too many targets");
		return;
	}
	// for every argument, check if it is a user or channel and print info accordingly
	for (std::map<int, Client*>::const_iterator it = _server->getClients().begin(); it != _server->getClients().end(); it++)
	{
		//! if user is hidden from WHOIS, break
		if (it->second->getNickname() == args[1])
		{
			Client *target = it->second;
			std::string nick = target->getNickname();
			std::string fullname = target->getFullname();
			std::string hostname = target->getHostname();
			client->send_response(RPL_WHOISUSER, client, nick + " " +fullname + " " + hostname);
			client->send_response(RPL_WHOISSERVER, client, servername);
			client->send_response(RPL_ENDOFWHOIS, client, "End of WHOIS list.");
			return;
		}
	}
	//! check if input is a channel
	//! for loop on server channels
    client->send_response(ERR_NOSUCHNICK, client, args[1] + " :No such nick/channel");
}
