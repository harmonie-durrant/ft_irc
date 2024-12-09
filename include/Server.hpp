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
#include "Client.hpp"

#define MAX_CLIENTS 1000		// valeur au hasard, a redefinir, mais il faut une valeur max pour listen()

class Server {

	typedef std::vector<pollfd>::iterator       pfd_iterator;	// pour les recherches dans le tableau _pfds sand devoir reecrire "std::vector<pollfd>::iterator" a chaque fois
	//typedef std::vector<Channel *>::iterator    channel_iterator;
    typedef std::map<int, Client *>::iterator   client_iterator;

	private:
		int 				_port;
		const std::string	_password;
		int					_server_fd;			// fd du socket serveur

		std::vector<pollfd>     _pfds;		// tableau de structures pollfd (1 pollfd pour le serveur et chaque clients)
		//std::vector<Channel *>  _channels;
		std::map<int, Client *> _clients;	//cle: int = le file descriptor du client, valeur: objet client (qui aura aussi son fd enregistre mais en private)
		

		/*
		struct pollfd {
			int fd;         			// Descripteur de fichier à surveiller
			short events;   			// Événements à surveiller
			short revents;  			// Événements détectés (rempli par poll() )
		};
		*/

	public:
		Server(int port, std::string password);
		~Server();

		int		create_socket();				// fonction qui creer et retourne le socket serveur
		void	client_connect();
		void	client_disconnect(int fd);
		void	client_message(int fd);


		int getPort() const;
		const std::string getPassword() const;
		//std::vector<Client> getClients() const;

		void start_server(void);

		//void addClient(Client client);
		//void removeClient(Client client);
};

#endif