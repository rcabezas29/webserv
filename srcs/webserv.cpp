#include "webserv.hpp"
#include "server.hpp"

int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	// if (argc == 2)
	// {
	// 	readfile(argv[1]);
	// }
	ws::server	serv;
	serv.connecting();
	return 0;
}
