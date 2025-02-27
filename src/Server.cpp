/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbryento <rbryento@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:57:44 by froque            #+#    #+#             */
/*   Updated: 2025/02/03 13:42:51 by rbryento         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port, std::string password, std::string servername): _port(port), _password(password), _servername(servername) {
	_max_global_channels = 10;
	_max_user_channels = 10;
	_commands["CAP"] = new Cap(this, false);
	_commands["PASS"] = new Pass(this, false);
	_commands["NICK"] = new Nick(this, false);
	_commands["USER"] = new User(this, false);
	_commands["PRIVMSG"] = new Privmsg(this, true);
	_commands["MODE"] = new Mode(this, true);
	_commands["PING"] = new Ping(this, false);
	_commands["QUIT"] = new Quit(this, false);
	_commands["WHOIS"] = new Whois(this, true);
	//for channels
	_commands["KICK"] = new Kick(this, true);
	_commands["INVITE"] = new Invite(this, true);
	_commands["TOPIC"] = new Topic(this, true);
	_commands["JOIN"] = new Join(this, false);
	_commands["PART"] = new Part(this, true);
	_commands["WHO"] = new Who(this, true);
	_server_fd = create_socket();
	std::cout << "Starting server on port " << port << std::endl;
}

Server::~Server() {
	std::cout << "Server shutting down..." << std::endl;

	for (command_iterator it = _commands.begin(); it != _commands.end(); it++)
		delete it->second;
	for (channel_iterator it = _channels.begin(); it != _channels.end(); it++)
		delete *it;
	std::cout << "Disconnecting clients..." << std::endl;
	client_iterator it = _clients.begin();
	while (it != _clients.end()) {
		this->client_disconnect(it->first);
		it = _clients.begin();
	}
	std::cout << "Closing server socket..." << std::endl;
	close(_server_fd);
	std::cout << "Server shut down, goodbye!" << std::endl;
}

int Server::getPort() const {
	return _port;
}

/*std::vector<Client> Server::getClients() const {
	return _clients;
}*/

std::string Server::getPassword() const {
	return _password;
}

void Server::client_disconnect(int fd) {
	std::cout << "INFO: Client fd " << fd << " disconnected" << std::endl;

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

void Server::client_connect(void) {
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

std::vector<std::string> split(const std::string str, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(str);
	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

int Server::handle_cache(std::string &buffer, Client *client, std::size_t bytes_read) {
	if (buffer[bytes_read - 1] != '\n')
	{
		client->appendCache(buffer);
		return 1;
	}
	buffer = client->getCache() + buffer;
	client->clearCache();
	return 0;
}

void Server::execute_command(std::vector<std::vector<std::string> > args, Client *client) {
	if (!client || args.empty())
		return;
	for (std::size_t i = 0; i < args.size(); i++)
	{
		int exists = false;
		for (client_iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			int cfd = client->getFd();
			if (it->first == cfd)
				exists = true;
		}
		if (!exists)
			return;
		Command *cmd = getCommand(args[i][0]);
		if (cmd == NULL)
		{
			std::cerr << "Command not found" << std::endl;
			client->send_response(ERR_UNKNOWNCOMMAND, client, args[i][0] + " :Unknown command");
			return;
		}
		if (cmd->auth_required() && !client->getAuth())
		{
			client->send_response(ERR_NOTREGISTERED, client, ":You have not registered" + args[i][0]);
			return;
		}
		cmd->execute(client, args[i]);
	}
}

Client *Server::get_client(int fd) {
	client_iterator it = _clients.find(fd);
	if (it == _clients.end())
	{
		std::cerr << "Client not found" << std::endl;
		return NULL;
	}
	return it->second;
}

void Server::client_message(int fd) {
	char c_buffer[1024] = {0};
	std::size_t bytes_read = recv(fd, c_buffer, sizeof(c_buffer), 0);
	if (bytes_read <= 0)
	{
		client_disconnect(fd);
		return;
	}
	std::string buffer(c_buffer);
	Client *client = get_client(fd);
	if (client == NULL)
		return;
	if (handle_cache(buffer, client, bytes_read) == 1)
		return;
	std::cout << "CLIENT (from " << fd << ") => " << buffer << std::endl;
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
	execute_command(args, client);
}

std::string	Server::strToLower(const std::string &input)
{
	std::string	inputLower = input;

	for (size_t i = 0; i < input.size(); i++)
	{
		inputLower[i] = std::tolower(static_cast<unsigned char>(input[i]));
	}
	return (inputLower);
}


bool	Server::nicknameExist(std::string nickname)
{
	client_iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		std::string tempNickname = strToLower(it->second->getNickname());
		if (tempNickname == strToLower(nickname))
			return true;
	}
	return false;
}

void Server::start_server(void)
{
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

int Server::create_socket() {

	int server_fd;
    struct sockaddr_in address;

    //Creer une socket pour le serveur
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
}

void Server::addChannel(Channel *channel) {
	channel->setCreationTime(getTimestamp());
	_channels.push_back(channel);
}

Client*		Server::get_client_by_nick(std::string client_nickname)
{
	for (client_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
        if (it->second->getNickname() == client_nickname)
            return it->second;
    }
    return NULL;
}

std::string Server::getServername() const {
	return _servername;
}

Channel*	Server::getChannel(std::string channel_name)
{
    for (channel_iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
        if ((*it)->getName() == channel_name)
            return *it;
    }
    return NULL;
}

void Server::removeChannel(Channel *channel)
{
	channel_iterator it = _channels.begin();
	while (it != _channels.end())
	{
		if (*it == channel)
		{
			delete *it;
			_channels.erase(it);
			return;
		}
		++it;
	}
}

int Server::getMaxGlobalChannels() const {
	return _max_global_channels;
}

int Server::getMaxUserChannels() const {
	return _max_user_channels;
}

Command	*Server::getCommand(std::string command) {
	Command *cmd = NULL;
	command_iterator it = _commands.find(command);
	if (it != _commands.end())
		cmd = it->second;
	return cmd;
}

const std::map<int, Client *>& Server::getClients() const {
	return _clients;
}

const std::vector<Channel *>& Server::getChannels() const {
	return _channels;
}

time_t Server::getTimestamp() {
	return time(NULL);
}

