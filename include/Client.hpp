#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>

class Client {
	private:
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