#include "webserv.hpp"
#include "server.hpp"
#include "server_config.hpp"

int	main(int argc, char **argv)
{
	std::vector<server_config>	config_servers;

	if (argc != 2)
		return -1; //lanzar excepcion de error
	try
	{
		config_servers = parse_config_file(argv[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	if (!check_paths("path/"))
		std::cout << "no vale" << std::endl;
	
	// ws::server	serv;
	// serv.connecting();
	return 0;
}