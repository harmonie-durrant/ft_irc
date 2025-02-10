#include "Command.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

Quit::Quit(Server* server, bool auth) : Command(server, auth) {}

Quit::~Quit() {}

void Quit::execute(Client* client, std::vector<std::string> args) {
	std::string quit_message = "Goodbye";
	if (args.size() > 1)
		quit_message = args[1];
	if (quit_message[0] == ':')
		quit_message.erase(0, 1);
	for (std::size_t i = 2; i < args.size(); i++)
	{
		quit_message = quit_message + " " + args[i];
	}
	if (client->getChannels().size() > 0)
	{
		for (std::size_t i = 0; i < client->getChannels().size(); i++)
		{
			Channel *channel = _server->getChannel(client->getChannels()[i]);
			if (!channel)
				return;
			channel->removeClient(client, quit_message);
		}
	}
	_server->client_disconnect(client->getFd());
}
