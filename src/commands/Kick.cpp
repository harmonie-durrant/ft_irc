/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguillet <fguillet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 17:08:37 by fguillet          #+#    #+#             */
/*   Updated: 2024/12/15 20:17:51 by fguillet         ###   ########.fr       */
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
    std::string targetNick = args[2];
    std::string reason = args.size() > 3 ? args[3] : "No reason";
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
    //Verifier si le client a expulser est dans le channel
    Client* target = channel->getClient(targetNick);
    if (!target) 
    {
        client->send_response(441, client, targetNick + " " + channelName + " :They aren't on that channel");
        return;
    }
    //Expulser l'individu susdite
    channel->kick(client, target, reason);
}