#include "request.hpp"

ws::request::request(void) {}

ws::request::~request(void) {}

void	ws::request::parse_start_line(std::string start_line) {
	std::vector<std::string>	mrv = ws::ft_split(start_line, ' ');

	this->_sl.method = mrv[0];
	this->_sl.request_target = mrv[1];
	this->_sl.http_version = mrv[2];
}