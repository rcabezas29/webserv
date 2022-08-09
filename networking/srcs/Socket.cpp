#include "Socket.hpp"

ws::Socket::Socket(int domain, int service_type, int protocol, int port, u_long interface) {
	this->address.sin_family = domain;
	this->address.sin_port = htons(port);
	this->address.sin_addr.s_addr = htonl(interface);
	this->sock = socket(domain, service_type, protocol);
}
