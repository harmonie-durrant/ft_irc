/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froque <froque@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 13:06:45 by froque            #+#    #+#             */
/*   Updated: 2025/01/10 13:06:48 by froque           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

Who::Who(Server* server, bool auth) : Command(server, auth) {}

Who::~Who() {}

void Who::execute(Client* client, std::vector<std::string> args) {
	if (args.empty() || args[1].empty() || args[1][0] != '#')
	{
		client->send_response(ERR_NOSUCHCHANNEL, client, " :You must specify a valid channel." + args[1] + ".");
		return;
	}
	std::string	channel_name = args[1];
	Channel* channel = _server->getChannel(channel_name);
	if (channel == NULL) {
		client->send_response(ERR_NOSUCHCHANNEL, client, channel_name + " :No such channel");
		return;
	}
	const std::vector<Client*>& clients = channel->getClients();
	for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
		Client* member = *it;
		std::string response;
		response = channel->getName() + " " + member->getUsername() + " " +
		member->getHostname() + " " + member->getServerName() + " " +
		member->getNickname() + " H";
		if (channel->isOperator(member))
			response += "@";
		response += " :0 " + member->getFullname();
		client->send_response(RPL_WHOREPLY, client, response);
	}
	client->send_response(RPL_ENDOFWHO, client, channel_name + " :End of WHO list");
}

// :<server> 352 <client_nick> <channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>
