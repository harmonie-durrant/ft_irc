#include "Command.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

User::User(Server* server, bool auth) : Command(server, auth) {}

User::~User() {}

void User::execute(Client* client, std::vector<std::string> args) {
	if (args.size() != 5)
	{
		client->setAuth(false);
		client->send_response(ERR_NEEDMOREPARAMS, _server, client, args[0] + " :Not enough parameters");
		return;
	}
	client->setUsername(args[1]);
	client->setHostname(args[2]);
	//! client->setServerName(args[3]); ???
	client->setFullname(args[4]);
	if (client->getNickname() != "" && client->getUsername() != "" && client->getHostname() != "" && client->getFullname() != "" && client->getPassOK() == true)
	{
		if (!client->getAuth())
			client->send_response(RPL_WELCOME, _server, client, ":Welcome to the Internet Relay Network " + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname());
		client->setAuth(true);
	}
}
