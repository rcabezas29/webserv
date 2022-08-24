#pragma once

#include "webserv.hpp"

namespace	ws
{
	class	Socket
	{
		private:
			int					_fd;
			struct sockaddr_in	*_address;
			
		public:
			Socket(int domain, int service_type, int protocol, int port, u_long interface);
			virtual	~Socket(void);

			int	get_fd(void) const;
			struct sockaddr_in	*get_address(void) const;
	};
};
