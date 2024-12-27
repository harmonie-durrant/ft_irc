/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguillet <fguillet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:05:51 by froque            #+#    #+#             */
/*   Updated: 2024/12/15 20:07:04 by fguillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <sys/socket.h>			// pour socket(), bind(), listen() et AF_INET, SOCKSTREAM
#include <netinet/in.h>			// pour htons(), ntohs(), structure sockadrr_in et INADDR_ANY
#include <unistd.h>				// pour close()
#include <fcntl.h>				// pour fcntl()
#include <poll.h>				// pour poll()
#include <algorithm>			// pour std::find()
#include <arpa/inet.h>          // pour inet_ntoa()
#include <csignal>				// pour signal()
#include <memory>				// pour std::unique_ptr
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "Client.hpp"

#define MAX_CLIENTS 1000		// valeur au hasard, a redefinir, mais il faut une valeur max pour listen()

// declarations to avoid not defined errors
class Command;

class Channel;

class Client;

class Server {

	typedef std::vector<pollfd>::iterator       pfd_iterator;	// pour les recherches dans le tableau _pfds sand devoir reecrire "std::vector<pollfd>::iterator" a chaque fois
	typedef std::vector<Channel *>::iterator    channel_iterator;
    typedef std::map<int, Client *>::iterator   client_iterator;
	typedef std::map<std::string, Command *>::iterator command_iterator;

	private:
		int 				_port;
		const std::string	_password;
		int					_server_fd;			// fd du socket serveur
		std::string			_servername;

		std::vector<pollfd>     _pfds;		// tableau de structures pollfd (1 pollfd pour le serveur et chaque clients)
		std::vector<Channel *>  _channels;
		std::map<int, Client *> _clients;	//cle: int = le file descriptor du client, valeur: objet client (qui aura aussi son fd enregistre mais en private)
		std::map<std::string, Command *> _commands;

		/*
		struct pollfd {
			int fd;         			// Descripteur de fichier à surveiller
			short events;   			// Événements à surveiller
			short revents;  			// Événements détectés (rempli par poll() )
		};
		*/

	public:
		Server(int port, std::string password, std::string servername);
		~Server();

		int		create_socket();				// fonction qui creer et retourne le socket serveur
		void	client_connect();
		void	client_disconnect(int fd);
		void	client_message(int fd);

		bool	nicknameExist(std::string nickname);
		int			handle_cache(std::string &buffer, Client *client, std::size_t bytes_read);
		void		execute_command(std::vector<std::vector<std::string> > args, Client *client);

		void		addChannel(Channel *channel);

		int 			getPort() const;
		std::string		getPassword() const;
		std::string		getServername() const;
		Channel*		getChannel(std::string channel_name);
		Client			*get_client(int fd);
		std::string	strToLower(const std::string &input);
		std::map<std::string, Command *>	getCommands() const;
		std::map<int, Client *>				getClients() const;
		std::vector<Channel *>				getChannels() const;
		Command								*getCommand(std::string command);
		Client								*get_client_by_nick(std::string nickname);

		void start_server(void);

		//void addClient(Client client);
		//void removeClient(Client client);

		// exception server closed
		class ServerClosed : public std::exception {
			public:
				virtual const char *what() const throw() {
					return "Server closed";
				}
		};
};

std::vector<std::string>	split(const std::string str, char delimiter);

#endif
