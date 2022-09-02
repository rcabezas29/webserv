#include "webserv.hpp"
#include "server.hpp"
#include "server_config.hpp"

int	main(int argc, char **argv)
{
	std::vector<server_config>	config_servers;

	if (argc != 2)
		return 1;
	config_servers = parse_config_file(argv[1]);
	// ws::server	serv;
	// serv.connecting();
	return 0;
}
