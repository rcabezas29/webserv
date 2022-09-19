#pragma once

#include "webserv.hpp"

namespace	ws
{
	struct status_line
	{
		std::string	http_version;
		std::string	reason_phrase;
		short		status_code;
	};
	
	class	response
	{
		private:
			status_line	_sl;
			std::map<std::string, std::string>	_headers;
			std::string	_body;

		public:
			response(void);
			~response(void);

			void		set_status_line(status_line stat);
			void		set_body(std::string b);
			void		set_headers(std::map<std::string, std::string>	h);

			std::string	get_body(void) const;

			std::string	response_to_text(void) const;
	};
}
