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
	shutdown(accept_fd, 2);
	return ;
}

std::string	ws::server::create_response(void) const {
	response		res;
	std::fstream	body_file;
	short			st_code = 404;


	std::cout << this->_req.get_host().size() << std::endl;
	if (this->_req.get_host().size() == 0)
		st_code = 400;
	else
	{
		for (std::vector<const location_config>::iterator it = this->_conf.locations.begin(); it != this->_conf.locations.end(); ++it)
		{
			if (it->path == this->_req.get_start_line().request_target)
				st_code = open_response_file(&body_file, *it);
		}
	}

	if (is_error_code(st_code))
		create_body_from_default_error_page(&body_file, st_code);

	res.set_status_line((status_line){this->_req.get_start_line().http_version, generate_reason_phrase(st_code), st_code});

	res.set_body(file_to_text(body_file));

	return res.response_to_text();
}

short		ws::server::open_response_file(std::fstream *body_file, location_config loc) const {
	if (check_if_dir(loc.root + "/" + loc.index))
	{
		if (loc.autoindex)
		{
			create_autoindex_file(body_file, loc.root + "/" + loc.index);
			return 200;
		}
		else
			return 403;
	}
	body_file->open(loc.root + "/" + loc.index);
	if (body_file->is_open())
		return 200;
	else
		return 404;
}

void	ws::server::create_body_from_default_error_page(std::fstream *file, short st_code) const {
	for (std::list<int>::const_iterator it = this->_conf.error_page.first.begin(); it != this->_conf.error_page.first.end(); ++it)
	{
		if (*it == st_code)
			file->open(this->_conf.error_page.second.c_str());
	}
	if (st_code == 403)
		file->open("html_files/403_forbidden.html");
	else if (st_code == 404)
		file->open("html_files/page_not_found.html");
}

void			ws::server::create_autoindex_file(std::fstream *file, std::string path) const {
	(void)path;
	file->open("html_files/temp_autoindex.html");
}

ws::Socket	ws::server::get_socket(void) const { return this->_sock; }
