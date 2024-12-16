#include "Command.hpp"

Join::Join(Server* server, bool auth) : Command(server, auth) {}

Join::~Join() {}

void Join::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response(461, _server, client, args[0] + " :Not enough parameters");
		return;
	}
	if (args[1] == "0") {
		// part all channels the client is in
		// send PART message to all users in the channels
		// remove client from all channels
		return;
	}
	if (args[1][0] != '#') {
		client->send_response(403, _server, client, args[1] + " :Channel needs to start with the channel prefix");
		return;
	}
	if (args[1].size() > 50) {
		client->send_response(403, _server, client, args[1] + " :Channel name too long");
		return;
	}
	if (args[1].find_first_of(" \a\b\f\n\r\t\v") != std::string::npos) {
		client->send_response(403, _server, client, args[1] + " :Channel name contains invalid characters");
		return;
	}
	// if any errors come up ERR_CHANNELISFULL, ERR_BADCHANNELKEY, ERR_INVITEONLYCHAN, or ERR_BANNEDFROMCHAN

	// create channel if doesn't exist and add user to it
	// broadcast JOIN message to all users in the channel
	// send client MODE, RPL_TOPIC, RPL_TOPICTIME, one or more RPL_NAMREPLY
}
