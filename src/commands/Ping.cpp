#include "Command.hpp"

Ping::Ping(Server* server, bool auth) : Command(server, auth) {}

Ping::~Ping() {}

void Ping::execute(Client* client, std::vector<std::string> args) {
	client->send_response("PONG " + args[1] + "\r\n");
}