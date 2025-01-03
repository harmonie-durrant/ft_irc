#include "Command.hpp"

Join::Join(Server* server, bool auth) : Command(server, auth) {}

Join::~Join() {}

void Join::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response(ERR_NEEDMOREPARAMS, client, args[0] + " :Not enough parameters");
		return;
	}
	if (args[1] == "0") {
		client->send_response(ERR_NOSUCHCHANNEL, client, args[1] + " :Invalid channel name");
		return;
	}
	if (args[1][0] != '#') {
		client->send_response(ERR_NOSUCHCHANNEL, client, args[1] + " :Channel needs to start with the channel prefix");
		return;
	}
	if (args[1].size() > 50) {
		client->send_response(ERR_NOSUCHCHANNEL, client, args[1] + " :Channel name too long");
		return;
	}
	if (args[1].find_first_of(" \a\b\f\n\r\t\v") != std::string::npos) {
		client->send_response(ERR_NOSUCHCHANNEL, client, args[1] + " :Channel name contains invalid characters");
		return;
	}
	if (client->getChannels().size() >= (std::size_t)_server->getMaxUserChannels()) {
		client->send_response(ERR_TOOMANYCHANNELS, client, args[1] + " :You are already in too many channels");
		return;
	}
	// create channel if doesn't exist and add user to it
	Channel *channel = _server->getChannel(args[1]);
	if (channel == NULL) {
		if (_server->getChannels().size() >= (std::size_t)_server->getMaxGlobalChannels()) {
			client->send_response(ERR_TOOMANYCHANNELS, client, args[1] + " :Too many channels");
			return;
		}
		channel = new Channel(args[1], "", client);
		_server->addChannel(channel);
		channel->addClient(client);
		channel->addOperator(client);
		channel->sendJoinSelf(client);
		return;
	}
	// check if user is already in channel
	if (channel->getClient(client->getNickname()) != NULL) {
		client->send_response(ERR_USERONCHANNEL, client, args[1] + " :You're already in that channel");
		return;
	}
	// check if channel is full
	if (channel->getLimit() == channel->getClients().size()) {
		client->send_response(ERR_CHANNELISFULL, client, args[1] + " :Channel is full");
		return;
	}
	// check if channel is invite only and user is invited
	if (channel->getInviteMode() && !channel->isInvited(client)) {
		client->send_response(ERR_INVITEONLYCHAN, client, args[1] + " :Channel is invite only");
		return;
	}
	// check if user is banned from channel
	// if (channel->is_banned(client)) {
	// 	client->send_response(ERR_BANNEDFROMCHAN, client, args[1] + " :You are banned from this channel");
	// 	return;
	// }
	// check if channel has a key and if user provided one
	if (channel->getKey() != "" && args.size() < 3) {
		client->send_response(ERR_BADCHANNELKEY, client, args[1] + " :Cannot join channel (+k) - bad key");
		return;
	}
	// check if user provided correct key
	if (args.size() == 3 && args[2] != channel->getKey()) {
		client->send_response(ERR_BADCHANNELKEY, client, args[1] + " :Cannot join channel (+k) - bad key");
		return;
	}
	channel->addClient(client);
	channel->sendJoinSelf(client);
	client->addChannel(channel->getName());
}
