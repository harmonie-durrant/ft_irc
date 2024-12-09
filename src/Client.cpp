#include "Client.hpp"

Client::Client() : _fd(0), _ip_addr(""), _port(0) {
}

Client::Client(int fd, std::string ip_addr, int port) : _fd(fd), _ip_addr(ip_addr), _port(port) {
}

Client::~Client() {
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

void Client::setFd(int fd) {
	_fd = fd;
}

void Client::setIpAddr(std::string ip_addr) {
	_ip_addr = ip_addr;
}