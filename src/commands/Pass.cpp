#include "Command.hpp"

Pass::Pass(Server* server, bool auth) : Command(server, auth) {}

Pass::~Pass() {}

void Pass::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response("ERR :Not enough parameters\r\n");
		return;
	}
	if (args[1] != _server->getPassword())
	{
		client->send_response("ERR :Password incorrect\r\n");
		_server->client_disconnect(client->getFd());
		return;
	}
	std::string nick = client->getNickname();
	client->send_response(":localhost 001 " + nick + "\r\n");
}
