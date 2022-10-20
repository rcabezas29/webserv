#include "Socket.hpp"

ws::Socket::Socket(int domain, int service_type, int protocol, int port, u_long interface)
{
	this->_address = new sockaddr_in();
	this->_address->sin_family = domain;
	this->_address->sin_port = htons(port);
	this->_address->sin_addr.s_addr = htonl(interface);

	if ((this->_fd = socket(domain, service_type, protocol)) == -1)
	{
		std::cerr << "Error at Socket" << std::endl;
		exit(1);
	}
	if (fcntl(this->_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Error at Fcntl" << std::endl;
		exit(1);
	}
	int	yes = 1;
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	{
		std::cerr << "Error at Setsockopt" << std::endl;
		exit(1);
	}
	if (bind(this->_fd, (struct sockaddr *)this->_address, sizeof(*this->_address)) < 0)
	{
		std::cerr << "Error at Bind" << std::endl;
		exit(1);
	}
	if (listen(this->_fd, 10) < 0)
	{
		std::cerr << "Error at Listen" << std::endl;
		exit(1);
	}
}

ws::Socket::~Socket(void) 
{
	// close(this->_fd);
}

int	ws::Socket::get_fd(void) const { return this->_fd; }

struct sockaddr_in	*ws::Socket::get_address(void) const { return this->_address; }
