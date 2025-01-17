/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeChannel.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguillet <fguillet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:05:44 by froque            #+#    #+#             */
/*   Updated: 2025/01/17 10:09:12 by froque           ###   ########.fr       */
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

class ModeChannel
{
	protected:
		class Channel*	_channel;
		bool	_auth;
		ModeChannel();
		ModeChannel(const ModeChannel &copy);

	public:
		/* CONSTRUCT DECONSTRUCT */
		explicit ModeChannel(class Channel* Channel, bool auth);
		virtual ~ModeChannel();
		/* GETTERS */
		bool 			auth_required() const;
		/* METHODS */
		virtual void	execute(class Client* client, std::vector<std::string> args) = 0;
};

/* COMMAND CLASSES */
