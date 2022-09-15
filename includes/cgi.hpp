#pragma once

#include "webserv.hpp"
#include "request.hpp"

namespace	ws
{
	struct cgi_meta_variables
	{
		int			content_length;
		std::string	content_type;
		std::string	gateway_interface;
		std::string	path_info;
		std::string	query_string;
		std::string	remote_addr;
		std::string	request_method;
		std::string	script_name;
		std::string	server_name;
		short		server_port;
		std::string	server_protocol;
		std::string	server_software;
	};

	class cgi
	{
		private:
			cgi_meta_variables	_cmv;
			std::string			_file;
			std::string			_program;

		public:
			cgi(std::pair<std::string, std::string> conf, server_config serv, std::string file, request req);
			~cgi(void);

			std::string	response(void);
			char		**set_vars_into_env(void) const;
	};
}
