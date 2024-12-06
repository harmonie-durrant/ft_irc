#include "Server.hpp"

Server::Server(int port, std::string password): _port(port), _password(password) {
	_clients = std::vector<Client>();
	std::cout << "Starting server on port " << port << std::endl;
}

Server::~Server() {
	std::cout << "Server shutting down" << std::endl;
}

int Server::getPort() const {
	return _port;
}

std::vector<Client> Server::getClients() const {
	return _clients;
}

void Server::setPort(int port) {
	_port = port;
}

void Server::setClients(std::vector<Client> clients) {
	_clients = clients;
}

void Server::addClient(Client client) {
	_clients.push_back(client);
}

void Server::removeClient(Client client) {
	std::vector<Client>::iterator it = _clients.begin();
	while (it != _clients.end()) {
		if (it->getFd() == client.getFd()) {
			_clients.erase(it);
			break;
		}
		it++;
	}
}
