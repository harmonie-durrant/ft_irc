#include "Command.hpp"

Cap::Cap(Server* server, bool auth) : Command(server, auth) {}

Cap::~Cap() {}

void Cap::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response("ERR :Not enough parameters\r\n");
		return;
	}
	if (args[1] == "LS")
	{
		// Server lists its capabilities
		client->send_response("CAP * LS :\r\n"); // Server replies with no capabilities
	}
	else if (args[1] == "REQ")
	{
		// Client requests a capability from the server
		client->send_response("CAP * ACK :\r\n"); // Server acknowledges the request but does not support any capabilities yet
	}
	else if (args[1] == "END")
	{
		// End of CAP negotiation from the client
		return;
	}
	else
	{
		client->send_response("ERR :Unknown CAP subcommand\r\n");
	}
}