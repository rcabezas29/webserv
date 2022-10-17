#include "server.hpp"

ws::server::server(void) : _sock(AF_INET, SOCK_STREAM, 0, 4242, INADDR_ANY) {}

ws::server::server(server_config conf) : _sock(AF_INET, SOCK_STREAM, 0, conf.listen, INADDR_ANY), _conf(conf), _active_sockets() {}

ws::server::~server(void) {}

std::string	ws::server::create_error_responses(short error_code) const
{
	ws::response	res;
	std::fstream	file;
	bool			found = false;

	for (std::list<int>::const_iterator it = this->_conf.error_page.first.begin(); it != this->_conf.error_page.first.end(); ++it)
		if (*it == error_code)
			found = true;

	if (found)
		file.open(this->_conf.error_page.second);
	else
		file.open("default_error_pages/" + std::to_string(error_code) + ".html");

	res.set_status_line((ws::status_line){"HTTP/1.1", ws::generate_reason_phrase(error_code), error_code});
	res.set_body(file_to_text(file));
	return res.response_to_text();
}

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

bool	ws::server::check_bad_request(void) const
{
	if (this->_req.get_start_line().http_version != "HTTP/1.1")
		return true;
	return false;
}

void	ws::server::connecting(int accept_fd, std::vector<struct pollfd> *pfds)
{
	char	buffer[30000] = {0};
	int		ret_recv;

	if ((ret_recv = recv(accept_fd, buffer, 30000, 0)) == -1)
		perror("In recv");
	this->parse_request(buffer);
	if (check_bad_request())
		send(accept_fd, create_error_responses(400).c_str(), create_error_responses(400).length(), 0);
	else
	{
		std::string res(this->create_response());
		send(accept_fd, res.c_str(), res.length(), 0);
	}
	if (ret_recv == 0 || (this->_req.get_headers().find("Connection") != this->_req.get_headers().end() && this->_req.get_headers()["Connection"] == "Close"))
	{
		ws::remove_fd_from_pollfd(pfds, accept_fd);
		close(accept_fd);
	}
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
			if (part)
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
	{
		if (path.find(it->first, path.size() - it->first.size() - 1) != std::string::npos)
			return true;
	}
	return false;
}

std::string		ws::server::create_response(void)
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

	if (loc.accepted_methods.find("DELETE") == loc.accepted_methods.end())
		return create_error_responses(405);
	else
	{
		std::string	path = this->is_absolute_path(this->_req.get_start_line().request_target);

		if (remove(path.c_str()) < 0)
			return create_error_responses(403);
		else
		{
			res.set_status_line((status_line){"HTTP/1.1", "OK", 200});
			return res.response_to_text();
		}
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
		return 201;
	}
}

std::string		ws::server::handle_multi_part(location_config loc) const
{
	response					res;
	res.set_status_line((status_line){"HTTP/1.1", "Internal Server Error", 500});
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
			body += *ite + "\n";
			++ite;
		}	

		for (std::map<std::string, std::string>::iterator mit = headers.begin(); mit != headers.end(); ++mit)
		{
			if (mit->first == "Content-Disposition" && mit->second.find("filename=") != std::string::npos)
			{
				short	stat_code = create_multipart_files(loc, mit->second.substr(mit->second.find("filename=") + 10, mit->second.size() - (mit->second.find("filename=") + 10) - 1), body);
				if (stat_code != 201)
					return create_error_responses(stat_code);
				else
					res.set_status_line((status_line){"HTTP/1.1", "Created", stat_code});
			}
		}
		headers.clear();
	}

	return res.response_to_text();
}

void		ws::server::handle_chunked_encoding(void)
{
	// Handle partitioned requests
	std::vector<std::string>			lines = ws::ft_split(this->_req.get_body(), "\n");
	std::vector<std::string>::iterator	it = lines.begin();
	std::string							body;
	
	while (it != lines.end() && *it != "0")
	{
		// if (*it == "")
		int	bytes = std::stoi(*it++);
		body += it->substr(0, bytes);
		it++;
	}
	this->_req.set_body(body);
}

std::string	ws::server::create_response_post(void)
{
	ws::response						res;
	std::map<std::string, std::string>	headers = this->_req.get_headers();
	location_config						loc = find_request_location(this->_req.get_start_line().request_target);
	std::string							path = this->is_absolute_path(this->_req.get_start_line().request_target);


	if (loc.accepted_methods.find("POST") == loc.accepted_methods.end())
		return create_error_responses(405);
	if (this->_req.get_body().size() > this->_conf.client_max_body_size)
		return create_error_responses(413);
	if (ws::map_value_exists(this->_req.get_headers(), "Transfer-Encoding", "chunked"))
		handle_chunked_encoding();
	if (ws::map_value_exists(this->_req.get_headers(), "Content-Type", "multipart/form-data") && loc.upload_directory.size() != 0)
		return handle_multi_part(loc);
	if (loc.cgi.size() <= 0)
		return create_error_responses(500);
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
	if (this->_req.get_body().size() == 2)
	{
		res.set_status_line((status_line){"HTTP/1.1", "OK", 200});
		return res.response_to_text();
	}
	return create_error_responses(501);
}

std::string	ws::server::create_response_get(void) const
{
	response		res;
	location_config	loc = find_request_location(this->_req.get_start_line().request_target);
	std::fstream	body_file;
	std::string		path = this->is_absolute_path(this->_req.get_start_line().request_target);
	short			st_code = 404;
	bool			absolute = false;

	if (loc.accepted_methods.find("GET") == loc.accepted_methods.end())
		return create_error_responses(405);
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
	if (st_code != 200)
		return create_error_responses(st_code);

	res.set_status_line((status_line){this->_req.get_start_line().http_version, "OK", 200});
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
	if (access(path.c_str(), F_OK) == -1)
		return 404;
	body_file->open(path);
	if (body_file->is_open())
		return 200;
	else
		return 403;
}

void			ws::server::create_autoindex_file(std::fstream *file, std::string path) const {
	
	std::string autoindex_path;

	autoindex_path = create_autoindex(path);
	file->open(autoindex_path);
}

void			ws::server::insert_fd_to_active_sockets(int fd)
{
	this->_active_sockets.insert(fd);
}

ws::Socket	ws::server::get_socket(void) const { return this->_sock; }

std::set<int>	ws::server::get_active_sockets(void) const { return this->_active_sockets; }