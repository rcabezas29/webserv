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
			std::string	_body;

		public:
			response(void);
			~response(void);

			void		set_status_line(status_line stat);
			void		set_body(std::string b);

			std::string	response_to_text(void) const;
	};
}
