#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <sys/socket.h>			// pour socket(), bind(), listen() et AF_INET, SOCKSTREAM
#include <netinet/in.h>			// pour htons(), structure sockadrr_in et INADDR_ANY
#include <unistd.h>				// pour close()
#include <fcntl.h>				// pour fcntl()
#include <poll.h>				// pour poll()
#include <algorithm>			// pour std::find()
#include "Client.hpp"

#define MAX_CLIENTS 1000		// valeur au hasard, a redefinir, mais il faut une valeur max pour listen()

class Server {

	typedef std::vector<pollfd>::iterator       pfd_iterator;	// pour les recherches dans le tableau _pfds

	private:
		int 				_port;
		const std::string	_password;
		std::vector<Client> _clients;
		int					_server_fd;			// fd du socket serveur

		std::vector<pollfd>     _pfds;		// tableau de structures pollfd (1 pollfd pour le serveur et chaque clients)

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
		std::vector<Client> getClients() const;

		void setPort(int port);
		void setClients(std::vector<Client> clients);

		void start_server(void);

		void addClient(Client client);
		void removeClient(Client client);
};

#endif
