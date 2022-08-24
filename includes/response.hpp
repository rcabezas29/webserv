#pragma once

#include "webserv.hpp"

namespace	ws
{
	struct status_line
	{
		std::string	http_version;
		int			status_code;
		std::string	reason_phrase;
	};
	
	class	response
	{
		private:
			status_line	_sl;

		public:

	};
}