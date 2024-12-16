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
	class Server*	_server;
	bool	_auth;
	Command();
	Command(const Command &copy);
public:
	// Constructors
	explicit Command(class Server* Server, bool auth);
	// Destructors
	virtual ~Command();
	// Overloaded Operators
	// Public Methods
	bool 			auth_required() const;
	virtual void	execute(class Client* client, std::vector<std::string> args) = 0;
	// Setters
	// Getter
	// Exceptions
};
// Ostream Overload

class Cap : public Command
{
public:
	Cap(class Server* server, bool auth);
	~Cap();

	void	execute(class Client* client, std::vector<std::string> args);
};

class Pass : public Command
{
public:
	Pass(class Server* server, bool auth);
	~Pass();

	void	execute(class Client* client, std::vector<std::string> args);
};

class Nick : public Command
{
public:
	Nick(class Server* server, bool auth);
	~Nick();

	void	execute(class Client* client, std::vector<std::string> args);
};

class User : public Command
{
public:
	User(class Server* server, bool auth);
	~User();

	void	execute(class Client* client, std::vector<std::string> args);
};

class Mode : public Command
{
public:
	Mode(class Server* server, bool auth);
	~Mode();

	void	execute(class Client* client, std::vector<std::string> args);
};

class Ping : public Command
{
public:
	Ping(class Server* server, bool auth);
	~Ping();

	void	execute(class Client* client, std::vector<std::string> args);
};

class Privmsg : public Command
{
public:
	Privmsg(class Server* server, bool auth);
	~Privmsg();

	void	execute(class Client* client, std::vector<std::string> args);
};

class Quit : public Command
{
public:
	Quit(class Server* server, bool auth);
	~Quit();

	void	execute(class Client* client, std::vector<std::string> args);
};

class Whois : public Command
{
public:
	Whois(class Server* server, bool auth);
	~Whois();

	void    execute(class Client* client, std::vector<std::string> args);
};

class Join : public Command
{
public:
	Join(class Server* server, bool auth);
	~Join();

	void	execute(class Client* client, std::vector<std::string> args);
};
