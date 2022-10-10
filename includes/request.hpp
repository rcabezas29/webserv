#pragma once

#include "webserv.hpp"
#include "utils.hpp"

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
			std::string 						_host;
			std::string							_body;
			std::string							_query;

		public:
			request(void);
			~request(void);

			start_line	get_start_line(void) const;
			std::string	get_host(void) const;
			std::string	get_body(void) const;
			std::map<std::string, std::string>	get_headers(void) const;
			std::string	get_query(void) const;

			void	set_body(std::string body);

			void	parse_header(std::string header);
			void	parse_start_line(std::string start_line);
	};
}
