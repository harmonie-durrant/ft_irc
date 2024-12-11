#include "Command.hpp"

Cap::Cap(Server* server, bool auth) : Command(server, auth) {}

Cap::~Cap() {}

void Cap::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response(461, _server, client, args[0] + " :Not enough parameters");
		return;
	}
	if (args[1] == "LS")
	{
		// Server lists its capabilities
		client->send_response(-1, _server, client, "CAP * LS :"); // Server replies with no capabilities
	}
	else if (args[1] == "REQ")
	{
		// Client requests a capability from the server
		client->send_response(-1, _server, client, "CAP * ACK :"); // Server acknowledges the request but does not support any capabilities yet
	}
	else if (args[1] == "END")
	{
		// End of CAP negotiation from the client
		return;
	}
	else
	{
		client->send_response(410, _server, client, args[1] + " :Invalid CAP subcommand");
	}
}