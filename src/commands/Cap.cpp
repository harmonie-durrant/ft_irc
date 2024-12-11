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
		client->send_response(-1, _server, client, "CAP * LS :");
		return;
	}
	if (args[1] == "REQ")
	{
		client->send_response(-1, _server, client, "CAP * ACK :");
		return;
	}
	if (args[1] == "END")
		return;
	client->send_response(410, _server, client, args[1] + " :Invalid CAP subcommand");
}
