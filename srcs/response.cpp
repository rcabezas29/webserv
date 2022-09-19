#include "response.hpp"

ws::response::response(void) {}

ws::response::~response(void) {}

void	ws::response::set_status_line(status_line stat) {
	this->_sl.http_version = stat.http_version;
	this->_sl.reason_phrase = stat.reason_phrase;
	this->_sl.status_code = stat.status_code;
}

void		ws::response::set_body(std::string b) { this->_body = b; }

void		ws::response::set_headers(std::map<std::string, std::string>	h) { this->_headers = h; }

std::string		ws::response::get_body(void) const { return this->_body; }

std::string	ws::response::response_to_text(void) const {
	std::string	res;

	res = this->_sl.http_version + " " + std::to_string(this->_sl.status_code) + " " + this->_sl.reason_phrase + "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
		res += it->first + ": " + it->second + "\r\n";
	res += "\r\n";
	res += this->_body;
	return res;
}
