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
		serverIrc = new Server(port, av[2]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	try
	{
		serverIrc->start_server();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	delete serverIrc;
}
