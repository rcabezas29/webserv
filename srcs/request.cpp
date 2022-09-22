#include "request.hpp"

ws::request::request(void) {}

ws::request::~request(void) {}

void	ws::request::parse_start_line(std::string start_line) {
	std::vector<std::string>	mrv = ws::ft_split(start_line, " ");

	this->_sl.method = mrv[0];
	this->_sl.request_target = mrv[1];
	this->_sl.http_version = mrv[2];
}

void	ws::request::parse_header(std::string header)
{
	if (header.substr(0, header.find(' ')) == "Host:")
		this->_host = header.substr(6, header.size());
	if (header.find(':') != std::string::npos)
		this->_headers[header.substr(0, header.find(':'))] = header.substr(header.find(':') + 2);
}

ws::start_line	ws::request::get_start_line(void) const { return this->_sl; }

std::string	ws::request::get_host(void) const { return this->_host; }

std::string	ws::request::get_body(void) const { return this->_body; }

std::map<std::string, std::string>	ws::request::get_headers(void) const { return this->_headers; }

void		ws::request::set_body(std::string body) { this->_body = body; }
