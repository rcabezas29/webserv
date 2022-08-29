#include "response.hpp"

ws::response::response(void) {}

ws::response::~response(void) {}

void	ws::response::set_status_line(status_line stat) {
    this->_sl.http_version = stat.http_version;
    this->_sl.reason_phrase = stat.reason_phrase;
    this->_sl.status_code = stat.status_code;
}
