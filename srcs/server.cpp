#include "server.hpp"

ws::server::server(void) : _sock(AF_INET, SOCK_STREAM, 0, 4242, INADDR_ANY) {}

ws::server::~server(void) {}

void	ws::server::connecting(void) {
	int accept_fd = 0;
	int addrlen = sizeof(this->_sock.get_address());
	char buffer[30000] = {0};

	while (true)
	{
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if ((accept_fd = accept(this->_sock.get_fd(), (struct sockaddr *)this->_sock.get_address(), (socklen_t *)&addrlen)) < 0)
		{
			perror("In accept");
			exit(1);
		}
		recv(accept_fd, buffer, 30000, 0);
		printf("-- THIS IS CONNECTION BUFFER -- \n%s\n", buffer);
		send(accept_fd, "HTTP/1.1 200 OK\r\n"
						"Content-type: text/html\r\n"
						"Content-length: 13\r\n"
						"\r\n"
						"<h1>hola</h1>"
			, 77, 0);
		printf("---------------Hello message sent----------------\n");
		shutdown(accept_fd, 2);
	}
	return ;
}