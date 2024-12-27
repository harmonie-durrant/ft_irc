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
		nickname = args[1];
		if (args[1][0] == ' ')
			nickname = nickname.substr(1);
		client->send_response(ERR_NICKNAMEINUSE, client, nickname + " " + nickname);
		return;
	}
	client->setNickname(nickname);
	client->send_response(-1, client, ":" + oldnick + " NICK " + client->getNickname());
}
