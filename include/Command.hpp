/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froque <froque@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:05:44 by froque            #+#    #+#             */
/*   Updated: 2024/12/10 13:05:46 by froque           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Header protection
#pragma once

// Includes
#include <iostream>
#include <string>
#include "Server.hpp"
#include "Client.hpp"

class Command
{
private:
	/* data */
protected:
	Server*	_server;
	bool	_auth;
	Command();
	Command(const &copy);
public:
	// Constructors
	explicit Command(Server* Server, bool auth);
	// Destructors
	virtual ~Command();
	// Overloaded Operators
	// Public Methods
	bool 			auth_required() const;
	virtual void	execute(Client* client, std::vector<std::string> args) = 0;
	// Setters
	// Getter
	// Exceptions
};
// Ostream Overload

class Quit : public Command
{
public:
	Quit(Server* server, bool auth);
	~Quit();

	void	execute(Client* client, std::vector<std::string> args);
};
