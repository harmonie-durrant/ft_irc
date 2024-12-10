#include "Command.hpp"

Nick::Nick(Server* server, bool auth) : Command(server, auth) {}

Nick::~Nick() {}

void Nick::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response("ERR :Not enough parameters\r\n");
		return;
	}
	std::cout << "TEST EEK" << args.size() << std::endl;
	client->setNickname(args[1]);
	client->send_response(":" + args[1] + " NICK " + args[1] + "\r\n");
}