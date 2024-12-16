#include "Command.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

Quit::Quit(Server* server, bool auth) : Command(server, auth) {}

Quit::~Quit() {}

void Quit::execute(Client* client, std::vector<std::string> args) {
	(void)args;
	_server->client_disconnect(client->getFd());
}
