#pragma once

#include "webserv.hpp"

struct limit_except
{
	std::list<std::string>							methods;
	std::list<std::string>							allowed_dirs;
	std::list<std::string>							deny_dirs;
};

struct location_cofig
{
	std::string										path;
	std::vector<limit_except>						limit_exceptions;
	bool											autoindex;
	std::string										default_error_file;
	std::string										root;
};

struct server_config
{
	std::string										server_name;
	std::list<std::pair<std::string, std::string> >	cgi;
	std::pair<std::list<int>, std::string>			error_page;
	long											client_max_body_size;
};
