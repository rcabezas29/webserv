#include "webserv.hpp"
#include "server.hpp"
#include "server_config.hpp"

void	add_servers_to_cluster(std::vector<ws::server>	*cluster, std::vector<server_config> config_servers)
{
	for (std::vector<server_config>::iterator it = config_servers.begin(); it != config_servers.end(); ++it)
	{
		ws::server	serv(*it);
		cluster->push_back(serv);
	}
}

void	add_fds_to_pollfd(struct pollfd *pfds, std::vector<ws::server> cluster)
{
	for (std::vector<ws::server>::size_type i = 0; i < cluster.size(); i++)
	{
		pfds[i].fd = cluster[i].get_socket().get_fd();
		pfds[i].events = POLLIN;
	}
}

int	main(int argc, char **argv)
{
	std::vector<server_config>	config_servers;
	std::vector<ws::server>		cluster;
	struct pollfd 				*pfds;

	if (argc != 2)
		return 1;
	try
	{
		config_servers = parse_config_file(argv[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		exit(1);
	}

	add_servers_to_cluster(&cluster, config_servers);

	pfds = new pollfd[cluster.size()];

	add_fds_to_pollfd(pfds, cluster);
	while (true)
	{
		if (poll(pfds, cluster.size(), INT32_MAX) == -1) {
			perror("poll");
			exit(1);
		}
		for (std::vector<ws::server>::size_type i = 0; i < cluster.size(); i++)
		{
			if (poll(pfds, cluster.size(), INT32_MAX) == -1) {
				perror("poll");
				exit(1);
			}
			for (std::vector<ws::server>::size_type i = 0; i < cluster.size(); i++)
			{
				if (pfds[i].revents & POLLIN)
					cluster[i].connecting();
			}
		}
	}
	return 0;
}