#include "server.hpp"

ws::server::server(void) : _sock(AF_INET, SOCK_STREAM, 0, 4242, INADDR_ANY) {}

ws::server::server(server_config conf) : _sock(AF_INET, SOCK_STREAM, 0, conf.listen, INADDR_ANY), _conf(conf) {}

ws::server::~server(void) {}

void	ws::server::parse_request(std::string request)
{
	std::vector<std::string>			request_lines = ws::ft_split(request, "\r\n");
	std::vector<std::string>::iterator	it = request_lines.begin();
	std::string							body;

	while (it != request_lines.end() && *it != "")
	{
		if (it == request_lines.begin())
			this->_req.parse_start_line(*it);
		else
			this->_req.parse_header(*it);
		++it;
	}
	while (it != request_lines.end())
	{
		body += *it + "\n";
		it++;
	}
	this->_req.set_body(body);
}

void	ws::server::connecting(void)
{
	int accept_fd = 0;
	int addrlen = sizeof(this->_sock.get_address());
	char buffer[30000] = {0};

	if ((accept_fd = accept(this->_sock.get_fd(), (struct sockaddr *)this->_sock.get_address(), (socklen_t *)&addrlen)) < 0)
	{
		perror("In accept");
		exit(1);
	}
	recv(accept_fd, buffer, 30000, 0);
	this->parse_request(buffer);
	std::string res(this->create_response());
	send(accept_fd, res.c_str(), res.length(), 0);
	shutdown(accept_fd, 2);
	this->_req.get_headers().clear();
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
	if (path_len > 1)
	{
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

bool	ws::server::check_if_cgi(location_config loc, std::string path) const
{
	for (std::map<std::string, std::string>::const_iterator it = loc.cgi.begin(); it !=  loc.cgi.end(); ++it)
		if (path.find(it->first, path.size() - it->first.size() - 1) != std::string::npos)
			return true;
	return false;
}

std::string		ws::server::create_response(void) const
{
	std::string	res;

	if (this->_req.get_start_line().method == "GET")
		return create_response_get();
	else if (this->_req.get_start_line().method == "POST")
		return create_response_post();
	else if (this->_req.get_start_line().method == "DELETE")
		return create_response_delete();
	else
		return "HTTP/1.1 501 Not Implemented";
}

std::string	ws::server::create_response_delete(void) const
{
	response		res;
	location_config	loc = find_request_location(this->_req.get_start_line().request_target);

	for (std::set<std::string>::iterator it = loc.accepted_methods.begin(); it != loc.accepted_methods.end(); ++it)
		std::cout << "hola" << *it << std::endl;

	if (loc.accepted_methods.find("DELETE") == loc.accepted_methods.end())
	{
		res.set_status_line((status_line){"HTTP/1.1", "Method Not Allowed", 405});
		return res.response_to_text();
	}
	else
	{
		std::string	path = this->is_absolute_path(this->_req.get_start_line().request_target);

		if (remove(path.c_str()) < 0)
			res.set_status_line((status_line){"HTTP/1.1", "Forbidden", 403});
		else
			res.set_status_line((status_line){"HTTP/1.1", "OK", 200});
		return res.response_to_text();
	}
}

location_config	ws::server::find_request_location(std::string request_target) const
{
	for (std::vector<location_config>::const_iterator it = this->_conf.locations.begin(); it != this->_conf.locations.end(); ++it)
		if (it->path == request_target)
			return *it;
	size_t pos = request_target.find_last_of('/');
	while (pos != 0)
	{
		for (std::vector<location_config>::const_iterator it = this->_conf.locations.begin(); it != this->_conf.locations.end(); ++it)
		{
			if (it->path == request_target.substr(0, pos))
				return *it;
		}
		pos = request_target.find_last_of('/', pos - 1);
	}
	for (std::vector<location_config>::const_iterator it = this->_conf.locations.begin(); it != this->_conf.locations.end(); ++it)
		if (it->path == "/")
			return *it;
	return (location_config){};
}

short	ws::server::create_multipart_files(location_config loc, std::string filename, std::string body) const
{
	std::ofstream	file;
	char			dir[FILENAME_MAX];

	getcwd(dir, FILENAME_MAX);

	std::string	filepath(dir);

	filepath += "/" + loc.upload_directory + "/" + filename;

	file.open(loc.upload_directory + "/" + filename);
	if (!file.is_open())
		return 403;
	else
	{
		file << body;
		return 200;
	}
}

std::string		ws::server::handle_multi_part(location_config loc) const
{
	response					res;
	std::string					boundary = this->_req.get_headers()["Content-Type"].substr(this->_req.get_headers()["Content-Type"].find('=') + 1);
	std::vector<std::string>	multiparts = ws::ft_split(this->_req.get_body(), "--" + boundary + "\n");

	for (std::vector<std::string>::iterator it = multiparts.begin() + 1; it != multiparts.end(); ++it)
	{
		std::map<std::string, std::string>	headers;
		std::vector<std::string>			lines = ws::ft_split(*it, "\n");
		std::vector<std::string>::iterator	ite = lines.begin();
		std::string							body;

		while (*ite != "")
		{
			headers.insert(std::make_pair(ite->substr(0, ite->find(':')), ite->substr(ite->find(':') + 2, ite->size())));
			++ite;
		}
		++ite;
		while (ite != lines.end() && *ite != "--" + boundary + "--")
		{
			body += *ite;
			++ite;
		}	

		for (std::map<std::string, std::string>::iterator mit = headers.begin(); mit != headers.end(); ++mit)
		{
			if (mit->first == "Content-Disposition" && mit->second.find("filename=") != std::string::npos)
			{
				short	stat_code = create_multipart_files(loc, mit->second.substr(mit->second.find("filename=") + 10, mit->second.size() - (mit->second.find("filename=") + 10) - 1), body);
				if (stat_code != 200)
				{
					res.set_status_line((status_line){"HTTP/1.1", "Forbidden", stat_code});
					return res.response_to_text();
				}
				else
					res.set_status_line((status_line){"HTTP/1.1", "OK", stat_code});
			}
		}
		headers.clear();
	}

	return res.response_to_text();
}

std::string	ws::server::create_response_post(void) const
{
	ws::response						res;
	std::map<std::string, std::string>	headers = this->_req.get_headers();
	location_config						loc = find_request_location(this->_req.get_start_line().request_target);
	std::string							path = this->is_absolute_path(this->_req.get_start_line().request_target);

	if (loc.accepted_methods.find("POST") == loc.accepted_methods.end())
	{
		res.set_status_line((status_line){"HTTP/1.1", "Method Not Allowed", 405});
		return res.response_to_text();
	}
	if (ws::map_value_exists(this->_req.get_headers(), "Content-Type", "multipart/form-data") && loc.upload_directory.size() != 0)
		return handle_multi_part(loc);
	if (loc.cgi.size() <= 0)
	{
		res.set_status_line((status_line){"HTTP/1.1", "Internal Server Error", 500});
		return res.response_to_text();
	}
	if (check_if_cgi(loc, path))
	{
		for (std::map<std::string, std::string>::const_iterator it = loc.cgi.begin(); it != loc.cgi.end(); ++it)
		{
			if (path.find(it->first, path.size() - it->first.size() - 1) != std::string::npos)
			{
				cgi bla(*it, this->_conf, path, this->_req);
				return bla.create_response();
			}
		}
	}
	res.set_status_line((status_line){"HTTP/1.1", "No ta implementao", 501});
	return res.response_to_text();
}

std::string	ws::server::create_response_get(void) const
{
	response		res;
	location_config	loc = find_request_location(this->_req.get_start_line().request_target);
	std::fstream	body_file;
	std::string		path = this->is_absolute_path(this->_req.get_start_line().request_target);
	short			st_code = 404;
	bool			absolute = false;

	if (check_if_cgi(loc, path))
	{
		for (std::map<std::string, std::string>::const_iterator it = loc.cgi.begin(); it != loc.cgi.end(); ++it)
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

		h["Content-Length"] = std::to_string(res.get_body().size());
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
