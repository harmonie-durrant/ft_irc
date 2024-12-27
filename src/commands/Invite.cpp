/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguillet <fguillet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 18:31:20 by fguillet          #+#    #+#             */
/*   Updated: 2024/12/15 20:17:43 by fguillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Invite::Invite(Server* server, bool auth) : Command(server, auth) {}

Invite::~Invite() {}
    
void Invite::execute(Client* client, std::vector<std::string> args)
{
    if (args.size() < 3) 
    {
        client->send_response(461, client, args[0] + " :Not enough parameters");
        return;
    }
    std::string targetNick = args[1];
    std::string channelName = args[2];
    Channel* channel = _server->getChannel(channelName);
    Client*  target = _server->get_client_by_nick(targetNick);
    
    if (!target) 
    {
        client->send_response(401, client, targetNick + " :No such nick/channel");
        return;
    }
    //Si le channel n'existe pas on peut quand meme envoyer une invitation (bizarerie RFC2812)
    if (!channel) 
    {
        client->send_response(-1, client, "You have invited " + targetNick + " to " + channelName);
        target->send_response(-1, target, ":" + client->getNickname() + " INVITE " + targetNick + " " + channelName);
        return;
    }
    //Si le client est deja dans le channel
    if (channel->isClient(target))
    {
        client->send_response(443, client, targetNick + " " + channelName + " :is already on channel");
        return;
    }
    //Si le channel est sur inviation seulement verifier que l'inviteur est operateur
    if (channel->getInviteMode() && !channel->isOperator(client))
    {
        client->send_response(482, client, channelName + " :You're not channel operator");
        return;
    }
    //channel->Invite(client, target, channelName);

    client->send_response(-1, client, "You have invited " + targetNick + " to " + channelName);
    target->send_response(-1, target, ":" + client->getNickname() + " INVITE " + targetNick + " " + channelName);
}