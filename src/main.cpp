#include "Server.hpp"

int ft_stoi(const char *str)
{
	std::stringstream ss(str);
	int num = 0;

	for (size_t i = 0; i < strlen(str); i++) {
		if (!isdigit(str[i]))
			return -1;
	}
	if (ss >> num)
		return num;
	return -1;
}

void signal_handler(int signum)
{
	std::cerr << "Signal " << signum << " received, exiting..." << std::endl;
	throw Server::ServerClosed(); //on leve une exception pour pouvoir fermer proprement le serveur après le try/catch
}

int main(int ac, char const *av[])
{
	int port = 0;
	Server *serverIrc = NULL;

	if (ac != 3) {
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return 1;
	}
	port = ft_stoi(av[1]);
	if (port < 1024 || port > 65535) {
		std::cerr << "Invalid port number" << std::endl;
		return 1;
	}
	if (strlen(av[2]) < 1) {
		std::cerr << "Invalid password" << std::endl;
		return 1;
	}
	try
	{
		signal(SIGINT, signal_handler); //gestion des signaux pour fermer proprement le serveur
		signal(SIGTERM, signal_handler);
		signal(SIGQUIT, signal_handler);
		signal(SIGKILL, signal_handler);
		serverIrc = new Server(port, av[2]);
        serverIrc->start_server(); //directement dans le meme bloc, si new Server() plante il ne tentera pas de lancer le serveur et faire un segfault
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	delete serverIrc;
}