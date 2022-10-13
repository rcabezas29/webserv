#include "Socket.hpp"

ws::Socket::Socket(int domain, int service_type, int protocol, int port, u_long interface)
{
	this->_address = new sockaddr_in();
	this->_address->sin_family = domain;
	this->_address->sin_port = htons(port);
	this->_address->sin_addr.s_addr = htonl(interface);

	if ((this->_fd = socket(domain, service_type, protocol)) == -1)
	{
		perror("Socket function error");
		exit(1);
	}
	if (fcntl(this->_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		perror("In setsockopt");
		exit(1);
	}
	int	yes = 1;
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	{
		perror("In setsockopt");
		exit(1);
	}
	if (bind(this->_fd, (struct sockaddr *)this->_address, sizeof(*this->_address)) < 0)
	{
		perror("In bind");
		exit(1);
	}
	if (listen(this->_fd, 10) < 0)
	{
		perror("In listen");
		exit(1);
	}
}

ws::Socket::~Socket(void) {}

int	ws::Socket::get_fd(void) const { return this->_fd; }

struct sockaddr_in	*ws::Socket::get_address(void) const { return this->_address; }
