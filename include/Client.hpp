#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>

class Client {
	private:
		int _fd;
		std::string _ip_addr;
	public:
		Client();
		~Client();

		int getFd() const;
		std::string getIpAddr() const;

		void setFd(int fd);
		void setIpAddr(std::string ip_addr);
};

#endif