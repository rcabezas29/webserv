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

std::string	ws::server::is_absolute_path(std::string path) const
{
	std::vector<const location_config>::iterator tmp = this->_conf.locations.begin();
	int 	path_len = path.length();

	if (path == "/")
	{
		for (std::vector<const location_config>::iterator it = this->_conf.locations.begin(); it != this->_conf.locations.end(); ++it)
		{
			if (it->path == "/")
				return (it->root + "/" + it->index);
		}
		this->_conf.locations.begin() = tmp;
	}	
	if (path_len > 1){
		std::string 	f_path = "";
		std::string		e_path = "";
		bool			part = true;
		int		n = 0;

		while (n < path_len)
		{
			if (path[n] == '/' && n != 0)
				part = false;
			if(part)
				f_path = f_path + path[n];
			else
				e_path = e_path + path[n];
			n++;
		}
		for (std::vector<const location_config>::iterator it = this->_conf.locations.begin(); it != this->_conf.locations.end(); ++it)
		{
			if (it->path == f_path)
			{
				if (f_path != "/") {
					std::fstream file;
					file.open((it->root + e_path).c_str());
					if (!file.is_open())
					{
						if (check_if_dir(it->root + e_path))
						{
							return (it->root + e_path);
						}
						file.open((it->root + e_path + "/" + it->index).c_str());
						if (!file.is_open())
						{
							if (check_if_dir(it->root + e_path + "/" + it->index))
							{
								return (it->root + e_path + "/" + it->index);
							}
							this->_conf.locations.begin() = tmp;
							return it->root + e_path;
						}
						else
							return (it->root + e_path + "/" + it->index);
					}
					file.close();
				}				
				this->_conf.locations.begin() = tmp;
				return (it->root + e_path + "/" + it->index);
			}
		}
	}
	for (std::vector<const location_config>::iterator it = this->_conf.locations.begin(); it != this->_conf.locations.end(); ++it)
	{
		if (it->path == "/")
		{
			this->_conf.locations.begin() = tmp;
			return (it->root + path);
		}
	}
	return path;
}

bool	ws::server::check_if_cgi(std::string path) const {
	for (std::map<std::string, std::string>::const_iterator it = this->_conf.cgi.begin(); it != this->_conf.cgi.end(); ++it)
	{
		if (path.find(it->first, path.size() - it->first.size() - 1) != std::string::npos)
			return true;
	}
	return false;
}

std::string	ws::server::create_response(void) const {
	response		res;
	std::fstream	body_file;
	short			st_code = 404;
	std::string		path;
	bool			absolute = false;
	
	path = this->is_absolute_path(this->_req.get_start_line().request_target);

	if (check_if_cgi(path))
	{
		for (std::map<std::string, std::string>::const_iterator it = this->_conf.cgi.begin(); it != this->_conf.cgi.end(); ++it)
		{
			if (path.find(it->first, path.size() - it->first.size() - 1) != std::string::npos)
			{
				cgi bla(*it, this->_conf, path, this->_req);
				return bla.create_response();
			}
		}
	}

	for (std::vector<const location_config>::iterator it = this->_conf.locations.begin(); it != this->_conf.locations.end(); ++it)
	{
		if (it->path == this->_req.get_start_line().request_target)
		{
			st_code = open_response_file(&body_file, *it, path);
			absolute = true;
		}
	}
	if (!absolute)
	{
		for (std::vector<const location_config>::iterator it = this->_conf.locations.begin(); it != this->_conf.locations.end(); ++it)
		{
			if (it->path == "/")
			{
				st_code = open_response_file(&body_file, *it, path);
				break ;
			}
		}
	}
	if (is_error_code(st_code))
		create_body_from_default_error_page(&body_file, st_code);

	res.set_status_line((status_line){this->_req.get_start_line().http_version, generate_reason_phrase(st_code), st_code});

	res.set_body(file_to_text(body_file));

	{
		std::map<std::string, std::string>	h;

		h["Content-Length"] = res.get_body().size();
		res.set_headers(h);
	}

	return res.response_to_text();
}

short		ws::server::open_response_file(std::fstream *body_file, location_config loc, std::string path) const 
{
	if (check_if_dir(path))
	{
		if (loc.autoindex)
		{
			create_autoindex_file(body_file, path);
			return 200;
		}
		else
			return 403;
	}
	body_file->open(path);
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
