/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froque <froque@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:06:18 by froque            #+#    #+#             */
/*   Updated: 2024/12/17 13:06:21 by froque           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

Part::Part(Server* server, bool auth) : Command(server, auth) {}

Part::~Part() {}

void Part::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response(ERR_NEEDMOREPARAMS, client, args[0] + " :Not enough parameters");
		return;
	}
	std::vector<std::string> target_channels = (args[1] == "0") ? client->getChannels() : split(args[1], ',');
	for (std::size_t i = 0; i < target_channels.size(); i++)
	{
		if (target_channels[i][0] != '#') {
			client->send_response(ERR_NOSUCHCHANNEL, client, target_channels[i] + " :Channel needs to start with the channel prefix");
			continue;
		}
		Channel *channel = _server->getChannel(target_channels[i]);
		if (channel == NULL) {
			client->send_response(ERR_NOSUCHCHANNEL, client, target_channels[i] + " :No such channel");
			continue;
		}
		if (channel->getClient(client->getNickname()) == NULL) {
			client->send_response(ERR_NOTONCHANNEL, client, target_channels[i] + " :You're not on that channel");
			continue;
		}
		// remove channel from user
		client->removeChannel(channel->getName());
		// remove user from channel sending PART to all users in channel
		channel->removeClient(client);
	}
}
