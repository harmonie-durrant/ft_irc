#include "Command.hpp"

Ping::Ping(Server* server, bool auth) : Command(server, auth) {}

Ping::~Ping() {}

void Ping::execute(Client* client, std::vector<std::string> args) {
	client->send_response(-1, _server, client, "PONG " + args[1]);
}
