#include "Command.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

Pass::Pass(Server* server, bool auth) : Command(server, auth) {}

Pass::~Pass() {}

void Pass::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response(ERR_NEEDMOREPARAMS, _server, client, args[0] + " :Not enough parameters");
		return;
	}
	if (args[1] != _server->getPassword())
	{
		client->send_response(ERR_PASSWDMISMATCH, _server, client, ":Password incorrect");
		_server->client_disconnect(client->getFd());
		return;
	}
	client->setAuth(true);
}
