#include "Server.hpp"

void Server::handle_cap(int fd, std::string buffer)
{
	const char *c_buffer = buffer.c_str();

	if (strncmp(c_buffer + 4, "LS", 2) == 0)
	{
		// Server lists its capabilities
		const char *response = "CAP * LS :\r\n"; // Server replies with no capabilities
		send(fd, response, strlen(response), 0);
	}
	else if (strncmp(c_buffer + 4, "REQ", 3) == 0)
	{
		// Client requests a capability from the server
		const char *response = "CAP * ACK :\r\n"; // Server acknowledges the request but does not support any capabilities yet
		send(fd, response, strlen(response), 0);
	}
	else if (strncmp(c_buffer + 4, "END", 3) == 0)
	{
		// End of CAP negotiation from the client
		return;
	}
}

static std::string get_param(std::string command, std::string buffer)
{
	std::string param;
	size_t pos = buffer.find(command);
	if (pos != std::string::npos)
	{
		pos += command.length() + 1;
		size_t end = buffer.find("\r\n", pos);
		param = buffer.substr(pos, end - pos);
	}
	return param;
}

void Server::handle_pass(int fd, std::string buffer)
{
	const char *c_buffer = buffer.c_str();
	std::string nick = get_param("NICK", buffer);
	std::string user_info = get_param("USER", buffer);
	if (strncmp(c_buffer + 5, _password.c_str(), _password.length()) == 0)
	{
		// TODO: handle user registration information
		std::string response = ":" + nick + " 001 " + nick + " :Welcome to ft_irc " + nick + "!\r\n";
		send(fd, response.c_str(), response.length(), 0);
	}
	else
	{
		std::string servername = user_info.substr(user_info.find(" ", user_info.find(" ", user_info.find(" ") + 1) + 1) + 1);
		std::string response = ":" + servername + " 464 " + nick + " :Password incorrect\r\n";
		send(fd, response.c_str(), response.length(), 0);
		client_disconnect(fd);
	}
}

void Server::handle_ping(int fd, std::string buffer)
{
	std::string nick = get_param("PING", buffer);
	std::string response = "PONG " + nick + "\r\n";
	send(fd, response.c_str(), response.length(), 0);
}

void Server::handle_mode(int fd, std::string buffer)
{
	std::string nick = get_param("MODE", buffer);
	std::string response = ":harmonie MODE " + nick + "\r\n";
	send(fd, response.c_str(), response.length(), 0);
}

void Server::handle_quit(int fd, std::string buffer)
{
	std::string nick = get_param("QUIT", buffer);
	std::string response = ":" + nick + " QUIT :Quit: " + nick + "\r\n";
	send(fd, response.c_str(), response.length(), 0);
	client_disconnect(fd);
}
