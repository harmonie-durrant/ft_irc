#include "Command.hpp"

Nick::Nick(Server* server, bool auth) : Command(server, auth) {}

Nick::~Nick() {}

void Nick::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response(461, _server, client, args[0] + " :Not enough parameters");
		return;
	}
	std::string oldnick = client->getNickname();
	std::string nickname = args[1];
	if (oldnick == nickname)
	{
		client->send_response(-1, _server, client, ":" + oldnick + " NICK " + oldnick);
		return;
	}
	if (_server->nicknameExist(nickname))
	{
		nickname = args[1];
		if (args[1][0] == ' ')
			nickname = nickname.substr(1);
		client->send_response(433, _server, client, nickname + " " + nickname);
		return;
	}
	client->setNickname(nickname);
	client->send_response(-1, _server, client, ":" + oldnick + " NICK " + client->getNickname());
	if (!client->getUsername().empty() && !client->getFullname().empty() && !client->getHostname().empty())
		client->send_response(001, _server, client, ":Welcome to the Internet Relay Network " + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname());
}
