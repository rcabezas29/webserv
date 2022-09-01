#pragma once

#include "webserv.hpp"
#include "Socket.hpp"
#include "request.hpp"
#include "response.hpp"
#include "utils.hpp"

namespace	ws
{
	class server
	{
		private:
			Socket			_sock;
			std::string 	_name;
			request			_req;

		public:
			server(void);
			~server(void);

			void	connecting(void);
			void	parse_request(std::string request);
			std::string	create_response(void) const;
	};
}
