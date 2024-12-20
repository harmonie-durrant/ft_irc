/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguillet <fguillet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:28:42 by froque            #+#    #+#             */
/*   Updated: 2024/12/15 18:05:15 by fguillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>

class Server;

class Client {
	private:
		std::string	_nickname;
		std::string	_username;
		std::string	_fullname;
		std::string	_hostname;
		bool		_auth;
		int			_fd;
		int			_port;		//apparemment peu utile de recuperer le port, on verra si on garde ou pas
		std::string _ip_addr;

	public:
		Client();
		Client(int fd, std::string ip_addr, int port);
		~Client();

		void send_response(int code, Server *server, Client *client, std::string msg);
		void send_message(std::string msg);

		// Getters
		std::string	getNickname(void) const;
		std::string getUsername(void) const;
		std::string getFullname(void) const;
		std::string getHostname(void) const;
		bool		getAuth() const;
		int			getFd() const;
		std::string getIpAddr() const;
		int			getPort() const;
		// Setters
		void setNickname(std::string nickname);
		void setUsername(std::string username);
		void setFullname(std::string fullname);
		void setHostname(std::string hostname);
		void setAuth(bool auth);
		void setFd(int fd);
		void setIpAddr(std::string ip_addr);
};

#endif
