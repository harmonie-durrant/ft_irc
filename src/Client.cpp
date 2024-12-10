/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froque <froque@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:57:34 by froque            #+#    #+#             */
/*   Updated: 2024/12/10 13:57:35 by froque           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : _fd(0), _port(0), _ip_addr("") {
}

Client::Client(int fd, std::string ip_addr, int port) : _fd(fd), _port(port), _ip_addr(ip_addr) {
}

Client::~Client() {
}

void Client::send_response(std::string response) {
	std::cout << "==> " << response << std::endl;
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

std::string Client::getNickname() const {
	return _nickname;
}

std::string Client::getUsername() const {
	return _username;
}

void Client::setFd(int fd) {
	_fd = fd;
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
