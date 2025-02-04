/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbryento <rbryento@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:05:30 by froque            #+#    #+#             */
/*   Updated: 2025/01/17 11:12:12 by rbryento         ###   ########.fr       */
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
#include "ModeChannel.hpp"

class ModeChannel;

class Channel
{
	typedef std::map<std::string, ModeChannel *>::iterator ModeChannel_iterator;

	private:
		std::string				_name;			// name of the channel
		std::string				_key;			// password for channel (empty = no password)
		std::string				_topic;			// topic of the channel
		std::vector<Client*>	_operators;		// list of the channel operators
		std::vector<Client*>	_clients;		// list of clients in the channel
        std::vector<Client*>	_guests_list;   // invited users for channel (if invite only mode)
		Server*					_server;		// server where the channel is

		size_t					_l;				//max users (0 = unlimited)
		bool					_i;				//invite only
		bool					_t;				//TOPIC command only for operators

		std::map<std::string, ModeChannel *>	_mode_channels;

		time_t					_topic_time;
		Client					*_topic_setter;

		time_t					_creation_time;
	public:
		Channel(std::string name, std::string key, Client* creator, Server* server);
		~Channel();

		/* SETTERS */
		void					setName(std::string name);
		void					setKey(std::string key);
		void					setTopic(std::string name);
		void					setLimit(size_t limit);
		void					setInviteMode(bool invite);
		void					setTopicMode(bool topic);

		/* BASIC GETTERS */
		std::string				getName() const;
		std::string				getKey() const;
		std::string				getTopic() const;
		std::vector<Client*>	getClients() const;
		size_t					getLimit() const;
		bool					getInviteMode() const;
		bool					getTopicMode() const;
		std::vector<Client*>	getOperators() const;
		Client*					getTopicSetter() const;
		time_t					getTopicTime() const;
		time_t					getCreationTime() const;

		/* ADVANCED GETTERS */
        Client*                 getClient(std::string client_nickname);
		const std::string		getNamesList();
		ModeChannel*			getModeChannel(std::string flag);

		/* METHODS */
		void					addClient(Client* client);
		void					removeClient(Client* client, std::string message);
        bool                    isClient(Client* client);
		bool					isInvited(Client* client);
		void					invite(Client* client);
		void					uninvite(Client* client);

        void                    addOperator(Client* client);
        void                    removeOperator(Client* client);
        bool                    isOperator(Client* client);

		void					sendJoinSelf(Client* client);
		void					broadcast(const std::string& message, Client* exclude);

		void					kick(Client* client, Client* target, const std::string reason);

		void					execute_mode_channel(Client* client, std::vector<std::string> args);

		void					setTopicSetter(Client* client);
		void					setTopicTime(time_t time);
		void					setCreationTime(time_t time);
};
