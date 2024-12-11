#include "Command.hpp"

Quit::Quit(Server* server, bool auth) : Command(server, auth) {}

Quit::~Quit() {}

void Quit::execute(Client* client, std::vector<std::string> args) {
	(void)args;
	_server->client_disconnect(client->getFd());
}
