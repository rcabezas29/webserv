#pragma once

#include "../webserv.hpp"

namespace	ws
{
	class	Socket
	{
		private:
			struct sockaddr_in	_address;
			std::string 		_name;
			int					_sock;
			
		public:
			Socket(std::string name, int domain, int service_type, int protocol,
				int port, u_long interface);
			virtual	~Socket(void);

			void	connecting(void);
	};
};
