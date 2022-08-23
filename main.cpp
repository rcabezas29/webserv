#include "webserv.hpp"

int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	ws::Socket	sock = ws::Socket("Server", AF_INET, SOCK_STREAM, 0, 4242, INADDR_ANY);

	sock.connecting();

	return 0;
}