#pragma once

#include "webserv.hpp"

struct location_config
{
	std::map<std::string, std::string>				cgi;
	std::set<std::string>							accepted_methods;
	std::string										path;
	std::string										index;
	std::string										root;
	std::string										upload_directory;
	bool											autoindex;
};

struct server_config
{
	std::string										server_name;
	std::pair<std::list<int>, std::string>			error_page;
	unsigned long									client_max_body_size;
	short											listen;
	std::vector<location_config>					locations;
};
