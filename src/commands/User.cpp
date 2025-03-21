#include "Command.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

User::User(Server* server, bool auth) : Command(server, auth) {}

User::~User() {}

void User::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 5)
	{
		client->setAuth(false);
		client->send_response(ERR_NEEDMOREPARAMS, client, args[0] + " :Not enough parameters");
		return;
	}
	if (client->getAuth())
	{
		client->send_response(ERR_ALREADYREGISTERED, client, ":You may not reregister (already registered)");
		return;
	}
	client->setUsername(args[1]);
	client->setHostname(args[2]);
	client->setServerName(args[3]);
	std::string full_name = args[4];
	for (size_t i = 0; i < args.size(); i++) {
		full_name = full_name + " " + args[i];
	}	
	client->setFullname(full_name);
	if (client->getPassOK() == false)
	{
		client->send_response(ERR_PASSWDMISMATCH, client, ":Password incorrect");
		client->send_response(-1, client, "ERROR :Closing Link: localhost (Bad Password)");
		_server->client_disconnect(client->getFd());
		return;

	}
	if (client->getNickname() != "" && client->getUsername() != "" && client->getHostname() != "" && client->getFullname() != "")
	{
		if (!client->getAuth())
			client->send_response(RPL_WELCOME, client, ":Welcome to the Internet Relay Network " + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname());
		client->setAuth(true);
	}
}
