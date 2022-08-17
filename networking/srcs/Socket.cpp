#include "../includes/Socket.hpp"

ws::Socket::Socket(std::string name, int domain, int service_type, int protocol, int port, u_long interface) : _name(name) {
	this->_address.sin_family = domain;
	this->_address.sin_port = htons(port);
	this->_address.sin_addr.s_addr = htonl(interface);
	if ((this->_sock = socket(domain, service_type, protocol)) == -1)
	{
		perror("Socket function error");
		exit(1);
	}
	if (bind(this->_sock, (struct sockaddr *)&this->_address, sizeof(this->_address)) < 0)
	{
		perror("In bind");
		exit(1);
	}
	if (listen(this->_sock, 10) < 0)
	{
		perror("In listen");
		exit(1);
	}
}

ws::Socket::~Socket(void) {

}

void	ws::Socket::connecting(void) {
	int accept_fd = 0;
	int addrlen = sizeof(this->_address);
	char buffer[30000] = {0};

	while (true)
	{
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if ((accept_fd = accept(this->_sock, (struct sockaddr *)&this->_address, (socklen_t *)&addrlen)) < 0)
		{
			perror("In accept");
			exit(1);
		}
		recv(accept_fd, buffer, 30000, 0);
		printf("-- THIS IS CONNECTION BUFFER -- \n%s\n", buffer);
		send(accept_fd, "hola", 4, 0);
		printf("------------------Hello message sent-------------------\n");
		close(accept_fd);
	}
	return ;
}
