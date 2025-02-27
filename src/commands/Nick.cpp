#include "Command.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

Nick::Nick(Server* server, bool auth) : Command(server, auth) {}

Nick::~Nick() {}

void Nick::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response(ERR_NEEDMOREPARAMS, client, args[0] + " :Not enough parameters");
		return;
	}
	std::string oldnick = client->getNickname();
	std::string nickname = args[1];
	if (oldnick == nickname)
	{
		client->send_response(-1, client, ":" + oldnick + " NICK " + oldnick);
		return;
	}
	if (_server->nicknameExist(nickname))
	{
		if (!client->getPassOK()) {
			client->setNickname(nickname);
			return;
		}
		args[1] = args[1] + "_";
		nickname = args[1];
		client->send_response(ERR_NICKNAMEINUSE, client, nickname + " " + nickname);
		return execute(client, args);
	}
	client->setNickname(nickname);
	if (!client->getPassOK()) {
		return;
	}
	client->send_response(-1, client, ":" + oldnick + " NICK " + client->getNickname());
	client->broadcast_to_channels(_server, ":" + oldnick + "!" + client->getUsername() + "@" + client->getHostname() +" NICK :" + client->getNickname(), client);
}
