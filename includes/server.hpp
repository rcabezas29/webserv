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

			bool			is_absolute_path(std::string path) const;
			void			connecting(void);
			void			parse_request(std::string request);
			std::string		create_response(void) const;
			short			open_response_file(std::fstream *body_file, location_config loc) const;
			void			create_body_from_default_error_page(std::fstream *file, short st_code) const;
			void			create_autoindex_file(std::fstream *file, std::string path) const;

			Socket	get_socket(void) const;
	};
}
