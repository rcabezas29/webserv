#include "cgi.hpp"

ws::cgi::cgi(std::pair<std::string, std::string> conf, std::string server_name, short port, std::string file, std::string remote_host)
{
	// this->_cmv.content_length = file.length();
    this->_cmv.gateway_interface = "CGI/1.1";
    this->_cmv.path_info = file;
    this->_cmv.query_string = "";
    this->_cmv.remote_addr = remote_host;
    this->_cmv.request_method = "GET";
    this->_cmv.script_name = conf.second;
    this->_cmv.server_name = server_name;
    this->_cmv.server_port = port;
    this->_cmv.server_protocol = "HTTP/1.1";
    this->_cmv.server_software = "webserv";

}

ws::cgi::~cgi(void) {}

std::string	ws::cgi::response(void)
{
    return "cgi";
}
