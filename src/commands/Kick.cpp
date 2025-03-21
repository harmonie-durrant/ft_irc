/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguillet <fguillet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 17:08:37 by fguillet          #+#    #+#             */
/*   Updated: 2025/02/03 13:04:24 by fguillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Kick::Kick(Server* server, bool auth) : Command(server, auth) {}

Kick::~Kick() {}
    
void Kick::execute(Client* client, std::vector<std::string> args)
{
    if (args.size() < 3) 
    {
        client->send_response(461, client, args[0] + " :Not enough parameters");
        return;
    }
    std::string channelName = args[1];
    std::string reason = "";
    if (args.size() > 3)
        reason = args[3];
    if (reason[0] == ':')
        reason.erase(0, 1);
    for (size_t i = 4; i < args.size(); i++)
        reason += " " + args[i];
    //Recuperer le canal
    Channel* channel = _server->getChannel(channelName);
    if (!channel)
    {
        client->send_response(403, client, channelName + " :No such channel");
        return;
    }
    //Verifier que l'utilisateur est operateur
    if (!channel->isOperator(client))
    {
        client->send_response(482, client, channelName + " :You're not channel operator");
        return;
    }
    Client* target = channel->getClient(args[2]);
    if (!target)
    {
        client->send_response(441, client, target->getNickname() + " " + channelName + " :They aren't on that channel");
        return;
    }
    //Expulser l'individu susdite
    channel->kick(client, target, reason);
}