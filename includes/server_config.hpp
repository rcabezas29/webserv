#pragma once

#include "webserv.hpp"

struct location_config
{
	std::string										path;
	std::list<std::string>							accepted_methods;
	bool											autoindex;
	std::string										index;
	std::string										root;
};

struct server_config
{
	std::string										server_name;
	std::map<std::string, std::string>				cgi;
	std::pair<std::list<int>, std::string>			error_page;
	long											client_max_body_size;
	short											listen;
	std::vector<location_config>					locations;
};
