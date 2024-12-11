#include "Command.hpp"

User::User(Server* server, bool auth) : Command(server, auth) {}

User::~User() {}

void User::execute(Client* client, std::vector<std::string> args) {
	if (args.size() != 5)
	{
		client->setAuth(false);
		client->send_response(461, _server, client, args[0] + " :Not enough parameters");
		return;
	}
	client->setUsername(args[1]);
	client->setHostname(args[2]);
	//! client->setServerName(args[3]); ???
	client->setFullname(args[4]);
	client->send_response(001, _server, client, ":Welcome to the Internet Relay Network " + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname());
}