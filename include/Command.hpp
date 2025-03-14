/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguillet <fguillet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:05:44 by froque            #+#    #+#             */
/*   Updated: 2024/12/15 17:22:00 by fguillet         ###   ########.fr       */
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

class Command
{
	protected:
		class Server*	_server;
		bool	_auth;
		Command();
		Command(const Command &copy);

	public:
		/* CONSTRUCT DECONSTRUCT */
		explicit Command(class Server* Server, bool auth);
		virtual ~Command();
		/* GETTERS */
		bool 			auth_required() const;
		/* METHODS */
		virtual void	execute(class Client* client, std::vector<std::string> args) = 0;
};

/* COMMAND CLASSES */

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
	void	modeChannel(class Client* client, class Channel* channel, std::vector<std::string> args);
};
class Part : public Command
{
public:
	Part(class Server* server, bool auth);
	~Part();

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

class Kick : public Command
{
public:
    Kick(class Server* server, bool auth);
	~Kick();

    void	execute(Client* client, std::vector<std::string> args);
};

class Invite : public Command
{
public:
    Invite(class Server* server, bool auth);
	~Invite();

    void	execute(Client* client, std::vector<std::string> args);
};

class Topic : public Command
{
public:
    Topic(class Server* server, bool auth);
	~Topic();

    void	execute(Client* client, std::vector<std::string> args);
};

class Join : public Command
{
public:
    Join(class Server* server, bool auth);
	~Join();

    void	execute(Client* client, std::vector<std::string> args);
};

class Who : public Command
{
public:
    Who(class Server* server, bool auth);
	~Who();

    void	execute(Client* client, std::vector<std::string> args);
};
