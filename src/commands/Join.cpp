#include "Command.hpp"

Join::Join(Server* server, bool auth) : Command(server, auth) {}

Join::~Join() {}

void Join::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response(ERR_NEEDMOREPARAMS, _server, client, args[0] + " :Not enough parameters");
		return;
	}
	if (args[1] == "0") {
		// for (channel_itterator it = client->get_channels().begin(); it != client->get_channels().end(); it++) {
		// 	Channel *channel = _server->get_channel(*it);
		// 	channel->remove_user(client); // sends PART to all users in channel
		// 	client->remove_channel(channel); // sends PART to client
		// }
		return;
	}
	if (args[1][0] != '#') {
		client->send_response(ERR_NOSUCHCHANNEL, _server, client, args[1] + " :Channel needs to start with the channel prefix");
		return;
	}
	if (args[1].size() > 50) {
		client->send_response(ERR_NOSUCHCHANNEL, _server, client, args[1] + " :Channel name too long");
		return;
	}
	if (args[1].find_first_of(" \a\b\f\n\r\t\v") != std::string::npos) {
		client->send_response(ERR_NOSUCHCHANNEL, _server, client, args[1] + " :Channel name contains invalid characters");
		return;
	}
	// if (client->get_channels().size() >= _server->get_max_channels()) {
	// 	client->send_response(ERR_TOOMANYCHANNELS, _server, client, args[1] + " :You are already in too many channels");
	// 	return;
	// }
	// create channel if doesn't exist and add user to it
	// Channel *channel = _server->get_channel(args[1]);
	// if (channel == nullptr) {
	// 	channel = new Channel(args[1]);
	// 	_server->add_channel(channel);
	// }
	// if (channel->is_user_in_channel(client)) {
	// 	client->send_response(ERR_ALREADYREGISTRED, _server, client, args[1] + " :You're already in that channel");
	// 	return;
	// }
	// if (channel->is_full()) {
	// 	client->send_response(ERR_CHANNELISFULL, _server, client, args[1] + " :Channel is full");
	// 	return;
	// }
	// if (channel->is_invite_only() && !channel->is_user_invited(client)) {
	// 	client->send_response(ERR_INVITEONLYCHAN, _server, client, args[1] + " :Channel is invite only");
	// 	return;
	// }
	// if (channel->is_banned(client)) {
	// 	client->send_response(ERR_BANNEDFROMCHAN, _server, client, args[1] + " :You are banned from this channel");
	// 	return;
	// }
	// if (channel->has_key() && args.size() < 3) {
	// 	client->send_response(ERR_BADCHANNELKEY, _server, client, args[1] + " :Channel key required");
	// 	return;
	// }
	// if (channel->has_key() && args[2] != channel->get_key()) {
	// 	client->send_response(ERR_BADCHANNELKEY, _server, client, args[1] + " :Invalid channel key");
	// 	return;
	// }
	// channel->add_user(client); // maybe handle errors here ERR_CHANNELISFULL, ERR_BADCHANNELKEY, ERR_INVITEONLYCHAN, or ERR_BANNEDFROMCHAN
	// client->add_channel(channel);
	// send client the channel info... MODE, RPL_TOPIC, RPL_TOPICTIME, one or more RPL_NAMREPLY, RPL_ENDOFNAMES ...
}
