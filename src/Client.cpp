#include "Client.hpp"

Client::Client() : _fd(0), _ip_addr("") {
}

Client::~Client() {
}

int Client::getFd() const {
	return _fd;
}

std::string Client::getIpAddr() const {
	return _ip_addr;
}

void Client::setFd(int fd) {
	_fd = fd;
}

void Client::setIpAddr(std::string ip_addr) {
	_ip_addr = ip_addr;
}
