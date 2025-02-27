/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbryento <rbryento@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:57:34 by froque            #+#    #+#             */
/*   Updated: 2025/02/03 11:10:39 by rbryento         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

Client::Client() : _nickname(""), _username(""), _fullname(""), _servername(""), _hostname(""), _auth(false), _passok(false), _fd(0), _port(0), _ip_addr(""), _cache("") {
} //! maybe disable this constructor with no parameters in the future

Client::Client(int fd, std::string ip_addr, int port) : _nickname(""), _username(""), _fullname(""), _hostname(""), _auth(false), _passok(false), _fd(fd), _port(port), _ip_addr(ip_addr), _cache("") {
}

Client::~Client() {
}

void Client::send_response(int code, Client *client, std::string msg) {
	std::string response;
	if (code == -1)
	{
		response = msg + "\r\n";
		std::cout << "CLIENT (to " << _fd << ") => " << response << std::endl;
		send(_fd, response.c_str(), response.length(), 0);
		return;
	}
	std::string servername = client->getServerName();
	std::string nick = client->getNickname();
	std::stringstream ss;
	ss << code;
	std::string s_code = ss.str();
	if (code > 999)
	{
		std::cerr << "ERR: Error while sending response: code must be between -1 and 999" << std::endl;
		return;
	}
	if (code < 10)
		s_code = "00" + s_code;
	else if (code < 100)
		s_code = "0" + s_code;
	response = ":" + servername + " " + s_code + " " + nick + " " + msg + "\r\n";
	if (nick.empty())
		response = ":" + servername + " " + s_code + " " + msg + "\r\n";
	std::cout << "CLIENT (to " << _fd << ") => " << response << std::endl;
	send(_fd, response.c_str(), response.length(), 0);
}

/* GETTERS */

int Client::getFd() const {
	return _fd;
}

std::string Client::getIpAddr() const {
	return _ip_addr;
}

int Client::getPort() const {
	return _port;
}

bool Client::getAuth() const {
	return _auth;
}

bool Client::getPassOK() const {
	return _passok;
}

std::string Client::getNickname() const {
	return _nickname;
}

std::string Client::getUsername() const {
	return _username;
}

std::string Client::getFullname() const {
	return _fullname;
}

std::string Client::getServerName() const {
	return _servername;
}

std::string Client::getHostname() const {
	return _hostname;
}

std::string Client::getCache() const {
	return _cache;
}

std::vector<std::string> Client::getChannels() const {
	return _channels;
}

/* SETTERS */

// channels

void	Client::broadcast_to_channels(Server *server, const std::string& message, Client* exclude)
{
	std::vector<std::string> user_cache;
	for (std::vector<std::string>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		Channel *channel = server->getChannel(*it);
		std::vector<Client*> clients = channel->getClients();
		std::vector<Client*>::iterator it_c = clients.begin();
		while (it_c != clients.end())
		{
			if (exclude != NULL && *it_c == exclude)
			{
				++it_c;
				continue;
			}
			std::string c_nick = (*it_c)->getNickname();
			if (std::find(user_cache.begin(), user_cache.end(), c_nick) != user_cache.end())
			{
				return;
			}
			(*it_c)->send_response(-1, *it_c, message);
			user_cache.push_back(c_nick);
			++it_c;
		}
	}
	user_cache.clear();
}

void Client::removeChannel(std::string channel) {
	for (std::vector<std::string>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		if (*it == channel) {
			_channels.erase(it);
			send_response(-1, this, ":" + _nickname + "!" + getUsername() + "@" + getHostname() + " PART " + channel + " :Goodbye");
			return;
		}
	}
	send_response(ERR_NOTONCHANNEL, this, channel + " :You're not on that channel");
}

void Client::addChannel(std::string channel) {
	_channels.push_back(channel);
}

void Client::removeChannels(Server* server) {
	for (std::vector<std::string>::iterator it = _channels.begin(); it != _channels.end();) {
		Channel* channel = server->getChannel(*it);
		if (!channel) {
			send_response(ERR_NOSUCHCHANNEL, this, *it + " :No such channel");
			it = _channels.erase(it);
			continue;
		}
		channel->removeClient(this, "Goodbye");
		it++;
	}
}

bool Client::inChannel(std::string channel) {
	std::vector<std::string>::iterator it = _channels.begin();
	while (it != _channels.end()) {
		if (*it == channel)
			return true;
		++it;
	}
	return false;
}

// cache

void Client::clearCache() {
	_cache = "";
}

void Client::appendCache(std::string str) {
	_cache = _cache + str;
}

// other setters

void Client::setFd(int fd) {
	_fd = fd;
}

void Client::setAuth(bool auth) {
	_auth = auth;
}

void Client::setPassOK(bool ok) {
	_passok = ok;
}

void Client::setIpAddr(std::string ip_addr) {
	_ip_addr = ip_addr;
}

void Client::setNickname(std::string nickname) {
	_nickname = nickname;
}

void Client::setUsername(std::string username) {
	_username = username;
}

void Client::setFullname(std::string fullname) {
	_fullname = fullname;
}

void Client::setServerName(std::string servername) {
	_servername = servername;
}

void Client::setHostname(std::string hostname) {
	_hostname = hostname;
}
