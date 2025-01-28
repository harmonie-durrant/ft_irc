#include "Command.hpp"

Join::Join(Server* server, bool auth) : Command(server, auth) {}

Join::~Join() {}

void Join::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
		return client->send_response(ERR_NEEDMOREPARAMS, client, args[0] + " :Not enough parameters");
	if (args[1] == "0")
		return client->send_response(ERR_NOSUCHCHANNEL, client, args[1] + " :Invalid channel name");
	if (args[1][0] != '#')
		return client->send_response(ERR_NOSUCHCHANNEL, client, args[1] + " :Channel needs to start with the channel prefix");
	if (args[1].size() > 50)
		return client->send_response(ERR_NOSUCHCHANNEL, client, args[1] + " :Channel name too long");
	if (args[1].find_first_of(" \a\b\f\n\r\t\v") != std::string::npos)
		return client->send_response(ERR_NOSUCHCHANNEL, client, args[1] + " :Channel name contains invalid characters");
	if (client->getChannels().size() >= (std::size_t)_server->getMaxUserChannels())
		return client->send_response(ERR_TOOMANYCHANNELS, client, args[1] + " :You are already in too many channels");
	// create channel if doesn't exist and add user to it
	Channel *channel = _server->getChannel(args[1]);
	if (channel == NULL) {
		if (_server->getChannels().size() >= (std::size_t)_server->getMaxGlobalChannels())
			return client->send_response(ERR_TOOMANYCHANNELS, client, args[1] + " :Too many channels");
		if (client->getChannels().size() >= (std::size_t)_server->getMaxUserChannels())
			return client->send_response(ERR_TOOMANYCHANNELS, client, args[1] + " :You are already in too many channels");
		channel = new Channel(args[1], "", client, _server);
		_server->addChannel(channel);
		channel->addClient(client);
		channel->addOperator(client);
		channel->sendJoinSelf(client);
		return;
	}
	if (channel->getClient(client->getNickname()) != NULL)
		return client->send_response(ERR_USERONCHANNEL, client, args[1] + " :You're already in that channel");
	if (channel->getLimit() != 0 && channel->getClients().size() >= channel->getLimit())
		return client->send_response(ERR_CHANNELISFULL, client, args[1] + " :Channel is full");
	if (channel->getInviteMode() && !channel->isInvited(client))
		return client->send_response(ERR_INVITEONLYCHAN, client, args[1] + " :Channel is invite only");
	// if (channel->is_banned(client))
	// 	return client->send_response(ERR_BANNEDFROMCHAN, client, args[1] + " :You are banned from this channel");
	if (channel->getKey() != "" && args.size() < 3)
		return client->send_response(ERR_BADCHANNELKEY, client, args[1] + " :Cannot join channel (+k) - bad key");
	if (args.size() == 3 && args[2] != channel->getKey())
		return client->send_response(ERR_BADCHANNELKEY, client, args[1] + " :Cannot join channel (+k) - bad key");
	if (client->getChannels().size() >= (std::size_t)_server->getMaxUserChannels())
		return client->send_response(ERR_TOOMANYCHANNELS, client, args[1] + " :You are already in too many channels");
	if (channel->isInvited(client))
		channel->uninvite(client);
	channel->addClient(client);
	channel->sendJoinSelf(client);
}
