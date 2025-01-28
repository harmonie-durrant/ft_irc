#include "Command.hpp"

Cap::Cap(Server* server, bool auth) : Command(server, auth) {}

Cap::~Cap() {}

void Cap::execute(Client* client, std::vector<std::string> args) {
	// this is so that CAP LS doesn't send unknown command
	if (args.size() < 2)
		return;
	if (args[1] == "END")
		return;
	if (args[1] == "LS")
		return client->send_response(-1, client, ":" + _server->getServername() + " CAP * LS :multi-prefix");
	if (args[1] == "REQ")
		return client->send_response(-1, client, ":" + _server->getServername() + " CAP * ACK :multi-prefix");
}
