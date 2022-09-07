#include "server.hpp"

ws::server::server(void) : _sock(AF_INET, SOCK_STREAM, 0, 4242, INADDR_ANY) {}

ws::server::server(server_config conf) : _sock(AF_INET, SOCK_STREAM, 0, conf.listen, INADDR_ANY), _conf(conf) {}

ws::server::~server(void) {}

void	ws::server::parse_request(std::string request) {
	std::vector<std::string>	request_lines = ws::ft_split(request, "\r\n");

	for (std::vector<std::string>::iterator it = request_lines.begin(); it != request_lines.end(); ++it)
	{
		if (it == request_lines.begin())
			this->_req.parse_start_line(*it);
		else
			this->_req.parse_header(*it);
	}
}

void	ws::server::connecting(void) {
	int accept_fd = 0;
	int addrlen = sizeof(this->_sock.get_address());
	char buffer[30000] = {0};

	if ((accept_fd = accept(this->_sock.get_fd(), (struct sockaddr *)this->_sock.get_address(), (socklen_t *)&addrlen)) < 0)
	{
		perror("In accept");
		exit(1);
	}
	recv(accept_fd, buffer, 30000, 0);
	printf("-- THIS IS CONNECTION BUFFER -- \n%s\n", buffer);
	this->parse_request(buffer);
	std::string res(this->create_response());
	send(accept_fd, res.c_str(), res.length(), 0);
	printf("---------------Hello message sent----------------\n");
	shutdown(accept_fd, 2);
	return ;
}

std::string	ws::server::create_response(void) const {

	response	res;
	short		st_code = 200;
	std::string	text;

	// open file to read and add to response as text
	res.set_status_line((status_line){this->_req.get_start_line().http_version, "OK", st_code});

	for (std::vector<const location_config>::iterator it = this->_conf.locations.begin(); it != this->_conf.locations.end(); ++it)
	{
		std::cout << "--> [" + (*it).path + "]" << std::endl;
		if ((*it).path == this->_req.get_start_line().request_target)
		{
			std::fstream	reading_file;
			std::string		body;
			std::string		line;

			reading_file.open((*it).index, std::ifstream::in);

			while (std::getline(reading_file, line))
				text += line + "\n";
		}
	}
	res.set_body(text);
	return res.response_to_text();
}

ws::Socket	ws::server::get_socket(void) const { return this->_sock; }
