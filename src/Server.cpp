/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froque <froque@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:57:44 by froque            #+#    #+#             */
/*   Updated: 2024/12/10 13:57:45 by froque           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port, std::string password): _port(port), _password(password) {
	_server_fd = create_socket();
	std::cout << "Starting server on port " << port << std::endl;
}

Server::~Server() {
	std::cout << "Server shutting down" << std::endl;
}

int Server::getPort() const {
	return _port;
}

/*std::vector<Client> Server::getClients() const {
	return _clients;
}*/

const std::string Server::getPassword() const {
	return _password;
}

void Server::client_disconnect(int fd)
{
	std::cout << "Client fd " << fd << " disconnected" << std::endl;

	client_iterator it_c = _clients.find(fd);
	if (it_c != _clients.end())
	{
		delete it_c->second;
		_clients.erase(it_c);
	}
    pfd_iterator it = _pfds.begin();
    while (it != _pfds.end())
    {
        if (it->fd == fd)
        {
            close(fd);
            _pfds.erase(it);
            return;
        }
        ++it;
    }
}

void Server::client_connect(void)
{
	int         client_fd;
    sockaddr_in addr;
    socklen_t   addr_len = sizeof(addr);

    if (_pfds.size() - 1 >= MAX_CLIENTS)
    {
        std::cerr << "Connection refused : max number of clients reached" << std::endl;
        return;
    }

	client_fd = accept(_server_fd, (struct sockaddr *)&addr, &addr_len);
	if (client_fd == -1)
        throw std::runtime_error("Error while accepting a new client");

    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1)
    {
        close(client_fd);
        throw std::runtime_error("Error while setting client socket to non blocking");
    }

	pollfd client_poll_fd = {client_fd, POLLIN, 0};
    _pfds.push_back(client_poll_fd);	// Ajouter le nouveau socket à la liste des sockets surveillés

    Client* client = new Client(client_fd, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    _clients[client_fd] = client; //Normalement le std::map classera automatiquement le tableau en fonction du fd client (=cle)

    std::cout << "Client with fd " << client_fd << " connected from ";
    std::cout << client->getIpAddr() << ":" << client->getPort() << std::endl;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(str);
	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

void Server::client_message(int fd)
{
	char c_buffer[1024] = {0};
	int bytes_read = recv(fd, c_buffer, sizeof(c_buffer), 0);
	if (bytes_read <= 0)
	{
		client_disconnect(fd);
		return;
	}
	std::string buffer(c_buffer);
	std::cout << "Message received from " << fd << " : " << buffer << std::endl;
	std::map<std::string, Command *> commands;
	commands["CAP"] = new Cap(this, false);
	commands["PASS"] = new Pass(this, false);
	commands["NICK"] = new Nick(this, false);
	commands["USER"] = new User(this, false);
	commands["PRIVMSG"] = new Privmsg(this, false);
	commands["MODE"] = new Mode(this, false);
	commands["PING"] = new Ping(this, false);
	commands["QUIT"] = new Quit(this, false);
	std::vector<std::string> command_args = split(buffer, '\n');
	std::vector<std::vector<std::string> > args;
	for (std::size_t i = 0; i < command_args.size(); i++)
	{
		args.push_back(split(command_args[i], ' '));
		for (std::size_t j = 0; j < args[i].size(); j++)
		{
			for (std::size_t k = 0; k < args[i][j].size(); k++)
			{
				if (args[i][j][k] == '\r')
				{
					args[i][j].erase(k, 1);
					break;
				}
			}
		}
	}
	for (std::size_t i = 0; i < command_args.size(); i++)
	{
		std::map<std::string, Command *>::iterator it;
		for (it = commands.begin(); it != commands.end(); it++)
		{
			if (args[i][0] == it->first)
			{
				client_iterator it_c = _clients.find(fd);
				if (it_c == _clients.end())
				{
					std::cerr << "Client not found" << std::endl;
					return;
				}
				Client *client = it_c->second;
				it->second->execute(client, args[i]);
				break;
			}
		}
	}
}

bool	Server::nicknameExist()
{
// verif si le nick name est deja pris si oui rajouter un _ a la fin
//
// avec la cmd nick mais aussi a la connection au serveur
// Mess a la connection
// Irssi: Your nickname is froque_
// Mess retour cmd nick
// Irssi: Your nick is in use by froque [~froque@evolu.net-7A04F0D5.ftth.fr.orangecustomers.net]
// voir si limite en nbre de char
}

void Server::start_server(void)
{
	//std::cout << "Server start : Hostname : ???? port :" << this->getPort() << std::endl;

	pollfd server_poll_fd = {_server_fd, POLLIN, 0};
    _pfds.push_back(server_poll_fd);

	while (true)
	{
        // Attendre que des evenements se produisent
        if (poll(&_pfds[0], _pfds.size(), -1) == -1)			// -1 = attendre indefiniment
            throw std::runtime_error("Error while polling");

        for (size_t i = 0; i < _pfds.size(); ++i)
        {
            pollfd pfd = _pfds[i];

            if (pfd.revents == 0)		//poll() n'a rien detecte pour ce fd
                continue;

            if (pfd.revents & POLLHUP) 		//poll() a detecte une deconnexion
			{
                client_disconnect(pfd.fd);
                --i;    //Reajuster pour regarder au meme indice au prochain tour de for() apres la suppression
                continue;
            }

            if (pfd.revents & POLLIN)
			{
                if (pfd.fd == _server_fd)		//POLLIN pour le server = un client veut se connecter
                    client_connect();
                else
                    client_message(pfd.fd);     //POLLIN pour un client = il veut envoyer qqchose au server (message/fichier)
			}
        }
	}
}
/*
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
}*/


int Server::create_socket() {

	int server_fd;
    struct sockaddr_in address;

    //Creer une socket pour le serveur
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
		throw std::runtime_error("Error while opening socket");

	//Option pour pouvoir reutiliser le port immediatement apres sa fermeture, sinon bug souvent
	int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	//Configurer le socket serveur en mode non bloquant, poll() le fait deja si on met le dernier parametre a 0 : poll(..., ..., 0)
	//mais apparemment bien de faire les 2, et on ne sait pas encore si on mettra 0 pour ce parametre de poll()
	if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Error while setting socket to non blocking");


    //Configurer l'adresse et le port
    address.sin_family = AF_INET;			//pour ipv4
    address.sin_addr.s_addr = INADDR_ANY;	//ecoute sur toutes les interfaces reseau locales dispo (127.0.0.1, adresse reseau local 192.168.x.x, adresse publique internet)
    address.sin_port = htons(_port);		//htons() pour conversion au format voulu par la structure sockadrr_in

	//Associer le socket a l'adresse et au port
    if (bind(server_fd, (const struct sockaddr *)&address, sizeof(address)) == -1) {
		close(server_fd);
        throw std::runtime_error("Error while binding socket");
    }

	//Met le socket en mode ecoute, pour recevoir des connexions clients, reception de messages, fichiers...
    if (listen(server_fd, MAX_CLIENTS) == -1) {
		close(server_fd);
        throw std::runtime_error("Error while listening on server socket");
    }

	return server_fd;

	//std::runtime_error pour pouvoir catch les throw dans les blocs catch(const std::exception& e) sans avoir
	//a creer une classe heritee de std::exception pour chaque erreur comme on faisait dans les CPP
}
