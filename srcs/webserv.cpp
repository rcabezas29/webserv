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

void	add_active_socket_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count)
{
	*fd_count += 1;

	*pfds = (struct pollfd *)realloc(*pfds, sizeof(**pfds) * (*fd_count));

    (*pfds)[*fd_count - 1].fd = newfd;
    (*pfds)[*fd_count - 1].events = POLLIN;
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

	int	pfds_size = cluster.size();

	pfds = new pollfd[pfds_size];

	add_fds_to_pollfd(pfds, cluster);

	while (true)
	{
		if (poll(pfds, pfds_size, INT32_MAX) == -1)
		{
			perror("poll");
			exit(1);
		}
		for (int i = 0; i < pfds_size; ++i)
		{
			if (pfds[i].revents & POLLIN)
			{
				for (std::vector<ws::server>::iterator it = cluster.begin(); it != cluster.end(); ++it)
				{
					if (it->get_socket().get_fd() == pfds[i].fd)
					{
						int accept_fd;
						int addrlen = sizeof(it->get_socket().get_address());
						if ((accept_fd = accept(pfds[i].fd, (struct sockaddr *)(it->get_socket().get_address()), (socklen_t *)&addrlen)) == -1)
							perror("In accept");
						else
						{
							fcntl(accept_fd, F_SETFL, O_NONBLOCK);

							add_active_socket_to_pfds(&pfds, accept_fd, &pfds_size);

							it->insert_fd_to_active_sockets(accept_fd);
						}
					}
					else
					{
						if (it->get_active_sockets().size() == 0)
							continue ;
						else
						{
							for (std::set<int>::iterator iter = it->get_active_sockets().begin(); iter != it->get_active_sockets().end(); ++iter)
							{
								if (*iter == pfds[i].fd)
									it->connecting(*iter);
							}
						}
					}
				}
			}
		}
	}
	return 0;
}