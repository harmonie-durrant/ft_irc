/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguillet <fguillet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:05:30 by froque            #+#    #+#             */
/*   Updated: 2024/12/16 12:54:12 by fguillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Header protection
#pragma once

// Includes
#include <iostream>
#include <string>
#include "Server.hpp"
#include "Client.hpp"

class Channel
{
	private:
		std::string				_name;
		std::string				_key;
		std::string				_topic;
		std::vector<Client*>	_operators;
		std::vector<Client*>	_clients;
        std::vector<Client*>	_guests_list;   //invited users for channel (if invite only mode)
		
		size_t					_l;		//max users (0 = unlimited)
		bool					_i;		//invite only
		bool					_t;		//TOPIC command only for operators

	public:
		Channel(std::string& name, std::string& key, Client* creator);
		~Channel();

		void					setName(std::string& name);
		void					setKey(std::string& key);
		void					setTopic(std::string& name);
		void					setLimit(size_t limit);

		std::string&			getName() const;
		std::string&			getKey() const;
		std::string&			getTopic() const;
		
        //Client*                 getClient(std::string client_nickname);
		size_t					getLimit() const;
		bool					getInviteMode() const;
		bool					getTopicMode() const;

		void					addClient(Client* client);
		void					removeClient(Client* client);
        void                    addOperator(Client* client);
        void                    removeOperator(Client* client);

        bool                    isOperator(Client* client);
        bool                    isClient(Client* client);

		void					broadcast(const std::string& message, Client* exclude);

		void					kick(Client* client, Client* target, const std::string& reason = "");
};

Channel::Channel(std::string& name, std::string& key, Client* creator): _name(name), _key(key),
	_topic(""), _l(0), _i(false), _t(false) {}

Channel::~Channel()	{}

void	Channel::setName(std::string name)	{ _name = name; }
void	Channel::setKey(std::string key)	{ _key = key; }
void	Channel::setTopic(std::string topic){ _topic = topic; }
void	Channel::setLimit(size_t limit)		{ _l = limit; }

std::string&	Channel::getName() const	{ return _name; }
std::string&	Channel::getKey() const		{ return _key; }
std::string&	Channel::getTopic() const	{ return _topic; }

size_t	Channel::getLimit() const			{ return _l; }
bool	Channel::getInviteMode() const		{ return _i; }
bool	Channel::getTopicMode() const		{ return _t; }

void	Channel::addClient(Client* client)
{
    if (std::find(_clients.begin(), _clients.end(), client) == _clients.end())
        _clients.push_back(client);
}

void	Channel::removeClient(Client* client)
{
    std::vector<Client*>::iterator it = std::find(_clients.begin(), _clients.end(), client);
    if (it != _clients.end())
        _clients.erase(it);
}

void    Channel::addOperator(Client* client)
{
    if (std::find(_operators.begin(), _operators.end(), client) == _operators.end())
        _operators.push_back(client);
}

void    Channel::removeOperator(Client* client)
{
    std::vector<Client*>::iterator it = std::find(_operators.begin(), _operators.end(), client);
    if (it != _operators.end())
        _operators.erase(it);
}

bool Channel::isOperator(Client* client)
{
    return std::find(_operators.begin(), _operators.end(), client) != _operators.end();
}

bool Channel::isClient(Client* client)
{
    return std::find(_clients.begin(), _clients.end(), client) != _clients.end();
}

void	Channel::broadcast(const std::string& message, Client* exclude)
{
    std::vector<Client*>::iterator it = _clients.begin();
    while (it != _clients.end())
    {
        if (exclude != NULL && *it == exclude)
        {
            ++it;
            continue;
        }
        (*it)->send_response(-1, NULL, *it, message);
        ++it;
    }
}

void Channel::kick(Client* kicker, Client* target, const std::string& reason = "") 
{
    removeClient(target);
    removeOperator(target);

    std::string message = ":" + kicker->getNickname() + " KICK " + _name + " " + target->getNickname() + " :" + reason;
    broadcast(message, NULL);

    target->send_response(-1, NULL, target, ":You have been kicked from " + _name + " by " + kicker->getNickname() + " (" + reason + ")");
}
