#include "Command.hpp"

Mode::Mode(Server* server, bool auth) : Command(server, auth) {}

Mode::~Mode() {}

void Mode::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2) {
		client->send_response(":harmonie 461 MODE :Not enough parameters\r\n");
		return;
	}
	std::string nick = client->getNickname();
	client->send_response(":" + nick + " MODE " + args[1] + "\r\n");
}