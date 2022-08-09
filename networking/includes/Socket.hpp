#pragma once

#include "webserv.hpp"

namespace	ws
{
	class Socket
	{
		private:
			struct sockaddr_in	address;
			int					sock;
			int					conn;
			
		public:
			Socket(int domain, int service_type, int protocol, int port, u_long interface);
			virtual	~Socket(void);
	};	
};
