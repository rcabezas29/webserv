#pragma once

#include "webserv.hpp"
#include "Socket.hpp"

namespace	ws
{
	class server
	{
		private:
			Socket				_sock;
			std::string 		_name;

		public:
			server(void);
			~server(void);

			void	connecting(void);
	};
}