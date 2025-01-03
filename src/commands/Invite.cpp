#include "Command.hpp"

Invite::Invite(Server* server, bool auth) : Command(server, auth) {}

Invite::~Invite() {}

void Invite::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 3)
	{
		client->send_response(ERR_NEEDMOREPARAMS, client, args[0] + " :Not enough parameters");
		return;
	}
	std::string nickname = args[1];
	std::string channel_name = args[2];
	Channel* channel = _server->getChannel(channel_name);
	if (channel == NULL)
		return client->send_response(ERR_NOSUCHCHANNEL, client, channel_name + " :No such channel");
	if (channel->isClient(client) == false)
		return client->send_response(ERR_NOTONCHANNEL, client, channel_name + " :You're not on that channel");
	if (channel->isOperator(client) == false)
		return client->send_response(ERR_CHANOPRIVSNEEDED, client, channel_name + " :You're not channel operator");
	Client* invitee = _server->get_client_by_nick(nickname);
	if (invitee == NULL)
		return client->send_response(ERR_NOSUCHNICK, client, nickname + " :No such nick/channel");
	if (channel->isInvited(invitee) == true)
		return client->send_response(ERR_USERONCHANNEL, client, nickname + " :User is already invited to channel");
	if (channel->isClient(invitee) == true)
		return client->send_response(ERR_USERONCHANNEL, client, nickname + " :User is already on channel");
	channel->invite(invitee);
	invitee->send_response(RPL_INVITING, client, nickname + " " + channel_name);
}
