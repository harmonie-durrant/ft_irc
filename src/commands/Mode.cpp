#include "Command.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

Mode::Mode(Server* server, bool auth) : Command(server, auth) {}

Mode::~Mode() {}

void Mode::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response(ERR_NEEDMOREPARAMS, client, args[0] + " :Not enough parameters");
		return;
	}
	std::string nick = client->getNickname();
	std::string servername = client->getServerName();
	if (args.size() == 2)
	{
		client->send_response(RPL_UMODEIS, client, nick + " :+i"); //! Send the user modes of the client
		return;
	}
	client->send_response(-1, client, ":" + servername + " MODE " + nick + " " + args[2]);
}
