#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include "Client.hpp"

class Server {
	private:
		int 				_port;
		const std::string	_password;
		std::vector<Client> _clients;

	public:
		Server(int port, std::string password);
		~Server();

		int getPort() const;
		std::vector<Client> getClients() const;

		void setPort(int port);
		void setClients(std::vector<Client> clients);

		void addClient(Client client);
		void removeClient(Client client);
};

#endif
