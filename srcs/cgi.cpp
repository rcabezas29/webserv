#include "cgi.hpp"
#include "stdlib.h"

extern char **environ;

ws::cgi::cgi(std::pair<std::string, std::string> conf, server_config serv, std::string file, request req) : _file(file), _program(conf.second)
{
	// this->_cmv.content_length = file.length();
	this->_cmv.gateway_interface = "CGI/1.1";
	this->_cmv.path_info = file;
	this->_cmv.query_string = "";
	this->_cmv.remote_addr = req.get_host();
	this->_cmv.request_method = req.get_start_line().method;
	this->_cmv.script_name = conf.second;
	this->_cmv.server_name = serv.server_name;
	this->_cmv.server_port = serv.listen;
	this->_cmv.server_protocol = req.get_start_line().http_version;
	this->_cmv.server_software = "webserv";
}

ws::cgi::~cgi(void) {}

char	**ws::cgi::set_vars_into_env(void) const
{
	std::string	str;
	char		**env = new char*[12];

	for (int i = 0; i < 12; ++i)
		env[i] = new char[100];

	env[0] = strdup("CONTENT_TYPE=text");
	env[1] = strdup("GATEWAY_INTERFACE=CGI/1.1");
	str = "PATH_INFO=" + this->_file;
	env[2] = strdup(str.c_str());
	str = "REMOTE_ADDR=" + this->_cmv.remote_addr;
	env[3] = strdup(str.c_str());
	str = "REQUEST_METHOD=" + this->_cmv.request_method;
	env[4] = strdup(str.c_str());
	str = "SCRIPT_FILENAME=" + this->_cmv.script_name;
	env[5] = strdup(str.c_str());
	str = "SERVER_NAME=" + this->_cmv.server_name;
	env[6] = strdup(str.c_str());
	str = "SERVER_PORT=" + std::to_string(this->_cmv.server_port);
	env[7] = strdup(str.c_str());
	str = "SERVER_PROTOCOL=" + this->_cmv.server_protocol;
	env[8] = strdup(str.c_str());
	str = "SERVER_SOFTWARE=" + this->_cmv.server_software;
	env[9] = strdup(str.c_str());
	// env[10] = NULL;
	return env;
}

std::string	ws::cgi::response(void)
{
	char	**env = set_vars_into_env();

	if (execle(this->_program.c_str(), this->_file.c_str(), env) < 0)
		std::cout << "No funca" << std::endl;

	return "cgi";
}
