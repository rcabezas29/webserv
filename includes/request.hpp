#pragma once

#include "webserv.hpp"

namespace	ws
{
	struct start_line
	{
		std::string	method;
		std::string	request_target;
		std::string	http_version;
	};

	class	request
	{
		private:
			start_line							_sl;
			std::map<std::string, std::string>	_headers;

		public:
			request(void);
			virtual	~request(void);

			void	parse_header(void);
			void	parse_start_line(void);
	};
}