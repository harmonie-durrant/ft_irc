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
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Channel
{
	private:
		std::string				_name;			// name of the channel
		std::string				_key;			// password for channel (empty = no password)
		std::string				_topic;			// topic of the channel
		std::vector<Client*>	_operators;		// list of the channel operators
		std::vector<Client*>	_clients;		// list of clients in the channel
        std::vector<Client*>	_guests_list;   // invited users for channel (if invite only mode)
		
		size_t					_l;				//max users (0 = unlimited)
		bool					_i;				//invite only
		bool					_t;				//TOPIC command only for operators

	public:
		Channel(std::string name, std::string key, Client* creator);
		~Channel();

		/* SETTERS */
		void					setName(std::string name);
		void					setKey(std::string key);
		void					setTopic(std::string name);
		void					setLimit(size_t limit);

		/* BASIC GETTERS */
		std::string				getName() const;
		std::string				getKey() const;
		std::string				getTopic() const;
		std::vector<Client*>	getClients() const;
		size_t					getLimit() const;
		bool					getInviteMode() const;
		bool					getTopicMode() const;

		/* ADVANCED GETTERS */
        Client*                 getClient(std::string client_nickname);
		std::string				getNamesList() const;

		/* METHODS */
		void					addClient(Client* client);
		void					removeClient(Client* client);
        bool                    isClient(Client* client);
		bool					isInvited(Client* client);
	
        void                    addOperator(Client* client);
        void                    removeOperator(Client* client);
        bool                    isOperator(Client* client);

		void					sendJoinSelf(Client* client);
		void					broadcast(const std::string& message, Client* exclude);

		void					kick(Client* client, Client* target, const std::string reason);
};
