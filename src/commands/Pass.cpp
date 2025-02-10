#include "Command.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

Pass::Pass(Server* server, bool auth) : Command(server, auth) {}

Pass::~Pass() {}

void Pass::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response(ERR_NEEDMOREPARAMS, client, args[0] + " :Not enough parameters");
		return;
	}
	if (client->getAuth())
	{
		client->send_response(ERR_ALREADYREGISTERED, client, ":Unauthorized command (already registered)");
		return;
	}
	if (args[1] != _server->getPassword())
	{
		client->setPassOK(false);
		//_server->client_disconnect(client->getFd());
		return;
	}
	client->setPassOK(true);
}
