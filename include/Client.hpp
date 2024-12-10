/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froque <froque@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 12:28:42 by froque            #+#    #+#             */
/*   Updated: 2024/12/10 12:28:46 by froque           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>

class Client {
	private:
		std::string	_nickname;
		std::string	_username;
		std::string	_fullname;
		int			_fd;
		int			_port;		//apparemment peu utile de recuperer le port, on verra si on garde ou pas
		std::string _ip_addr;

	public:
		Client();
		Client(int fd, std::string ip_addr, int port);
		~Client();

		int getFd() const;
		std::string getIpAddr() const;
		int getPort() const;

		void setFd(int fd);
		void setIpAddr(std::string ip_addr);
};

#endif
