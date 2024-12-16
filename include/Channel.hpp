/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froque <froque@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:05:30 by froque            #+#    #+#             */
/*   Updated: 2024/12/10 13:05:32 by froque           ###   ########.fr       */
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
	Client*					_operator;
	std::vector<Client *>	_clients;
	std::string				_topic;
	int						_limit;
public:
	// Constructors
	Channel(/* args */);
	// Destructors
	~Channel();
	// Overloaded Operators
	// Public Methods
	// Setters
	void		setName(std::string name);
	void		setTopic(class Client *client, std::string topic);
	void		setLimit(int limit);
	// Getter
	std::string	getName() const;
	std::string	getTopic() const;
	Client*		getOperator() const;
	int			getLimit() const;
	// Exceptions
};
// Ostream Overload
