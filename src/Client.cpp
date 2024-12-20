/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguillet <fguillet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:57:34 by froque            #+#    #+#             */
/*   Updated: 2024/12/16 12:54:45 by fguillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

Client::Client() : _auth(false), _fd(0), _port(0), _ip_addr("") {
}

Client::Client(int fd, std::string ip_addr, int port) : _auth(false), _fd(fd), _port(port), _ip_addr(ip_addr) {
}

Client::~Client() {
}

void Client::send_response(int code, Server *server, Client *client, std::string msg) {
	std::string response;
	if (code == -1)
	{
		response = msg + "\r\n";
		std::cout << _fd << "==> " << response << std::endl;
		send(_fd, response.c_str(), response.length(), 0);
		return;
	}
	std::string servername = server->getServername();
	std::string nick = client->getNickname();
	std::stringstream ss;
	ss << code;
	std::string s_code = ss.str();
	if (code > 999)
	{
		std::cerr << "Error: code must be between -1 and 999" << std::endl;
		response = ":" + servername + " 400 " + nick + ": Unknown Error\r\n";
		std::cout << "==> " << response << std::endl;
		send(_fd, response.c_str(), response.length(), 0);
		return;
	}
	if (code < 10)
		s_code = "00" + s_code;
	else if (code < 100)
		s_code = "0" + s_code;
	response = ":" + servername + " " + s_code + " " + nick + " " + msg + "\r\n";
	std::cout << _fd << "==> " << response << std::endl;
	send(_fd, response.c_str(), response.length(), 0);
}

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

std::string Client::getNickname() const {
	return _nickname;
}

std::string Client::getUsername() const {
	return _username;
}

std::string Client::getFullname() const {
	return _fullname;
}

std::string Client::getHostname() const {
	return _hostname;
}

void Client::setFd(int fd) {
	_fd = fd;
}

void Client::setAuth(bool auth) {
	_auth = auth;
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

void Client::setHostname(std::string hostname) {
	_hostname = hostname;
}
