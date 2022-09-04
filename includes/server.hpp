#pragma once

#include "webserv.hpp"
#include "Socket.hpp"
#include "request.hpp"
#include "response.hpp"
#include "utils.hpp"
#include "server_config.hpp"

namespace	ws
{
	class server
	{
		private:
			Socket			_sock;
			server_config	_conf;
			request			_req;

		public:
			server(void);
			server(server_config conf);
			~server(void);

			void		connecting(void);
			void		parse_request(std::string request);
			std::string	create_response(void) const;

			Socket	get_socket(void) const;
	};
}
