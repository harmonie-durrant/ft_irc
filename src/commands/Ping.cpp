#include "Command.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

Ping::Ping(Server* server, bool auth) : Command(server, auth) {}

Ping::~Ping() {}

void Ping::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response(ERR_NOORIGIN, client, args[0] + " :No origin specified");
		return;
	}
	client->send_response(-1, client, "PONG " + args[1]);
}
