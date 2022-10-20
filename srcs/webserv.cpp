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

void	add_fds_to_pollfd(std::vector<struct pollfd> &pfds, std::vector<ws::server> cluster)
{
	for (std::vector<ws::server>::size_type i = 0; i < cluster.size(); i++)
	{
		struct pollfd	pfd;
		pfd.fd = cluster[i].get_socket().get_fd();
		pfd.events = POLLIN;
		pfds.push_back(pfd);
	}
}

void	add_active_socket_to_pfds(std::vector<struct pollfd> &pfds, int newfd)
{
	struct pollfd	pfd;

	pfd.fd = newfd;
	pfd.events = POLLIN;
	pfds.push_back(pfd);
}

int	main(int argc, char **argv)
{
	std::vector<server_config>	config_servers;
	std::vector<ws::server>		cluster;
	std::vector<struct pollfd> 	pfds;

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
	add_fds_to_pollfd(pfds, cluster);
	while (true)
	{
		if (poll(&pfds[0], pfds.size(), INT32_MAX) == -1)
		{
			std::cerr << "Poll error" << std::endl;
			exit(1);
		}
		for (size_t i = 0; i < pfds.size(); ++i)
		{
			if (pfds[i].revents & POLLIN)
			{
				for (std::vector<ws::server>::iterator it = cluster.begin(); it != cluster.end(); ++it)
				{
					try
					{
						pfds.at(i);
					}
					catch(const std::exception& e)
					{
						break ;
					}
					if (it->get_socket().get_fd() == pfds[i].fd)
					{
						int accept_fd;
						int addrlen = sizeof(it->get_socket().get_address());
						if ((accept_fd = accept(pfds[i].fd, (struct sockaddr *)(it->get_socket().get_address()), (socklen_t *)&addrlen)) == -1)
							std::cerr << "Fail in accept" << std::endl;
						else
						{
							fcntl(accept_fd, F_SETFL, O_NONBLOCK);

							add_active_socket_to_pfds(pfds, accept_fd);

							it->insert_fd_to_active_sockets(accept_fd);
						}
					}
					else
					{
						if (it->get_active_sockets().size() == 0)
							continue ;
						else
						{
							std::set<int>	fds = it->get_active_sockets();
							for (std::set<int>::iterator iter = fds.begin(); iter != fds.end(); ++iter)
							{
								if (*iter == pfds[i].fd)
								{
									it->connecting(*iter, &pfds);
									break ;
								}
							}
						}
					}
				}
			}
		}
	}
	std::cerr << "Como puede ser" << std::endl;
	return 0;
}