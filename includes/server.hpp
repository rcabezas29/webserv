#pragma once

#include "webserv.hpp"
#include "Socket.hpp"
#include "request.hpp"
#include "response.hpp"
#include "utils.hpp"
#include "server_config.hpp"
#include "cgi.hpp"
#include "autoindex.hpp"

namespace	ws
{
	class server
	{
		private:
			Socket			_sock;
			server_config	_conf;
			request			_req;
			std::set<int>	_active_sockets;

		public:
			server(void);
			server(server_config conf);
			~server(void);

			std::string		is_absolute_path(std::string path) const;
			void			connecting(int accept_fd);
			void			parse_request(std::string request);
			std::string		create_response(void);
			std::string		create_response_get(void) const;
			std::string		create_response_post(void);
			std::string		create_response_delete(void) const;
			short			open_response_file(std::fstream *body_file, location_config loc, std::string path) const;
			void			create_autoindex_file(std::fstream *file, std::string path) const;
			location_config	find_request_location(std::string request_target) const;
			std::string		handle_multi_part(location_config loc) const;
			void			handle_chunked_encoding(void);
			short			create_multipart_files(location_config loc, std::string filename, std::string body) const;
			bool			check_if_cgi(location_config loc, std::string path) const;
			std::string		create_error_responses(short error_code) const;
			Socket			get_socket(void) const;
			bool			check_bad_request(void) const;
			std::set<int>	get_active_sockets(void) const;
			void			insert_fd_to_active_sockets(int fd);
	};
}
