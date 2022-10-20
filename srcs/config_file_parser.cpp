#include "webserv.hpp"
#include "server_config.hpp"
#include "utils.hpp"

location_config	parse_location(std::fstream *file, std::string path)
{
	location_config	lc;
	std::string		line;

	lc.path = path;
	while (std::getline(*file, line) && line != "    }")
	{
		line.resize(line.length() - 1);
		std::vector<std::string>	splitted_line = ws::ft_split(line, " ");

		if (splitted_line[0] == "index")
			lc.index = splitted_line[1];
		else if (splitted_line[0] == "autoindex")
			splitted_line[1] == "on" ? lc.autoindex = true : lc.autoindex = false;
		else if (splitted_line[0] == "root")
			lc.root = splitted_line[1];
		else if (splitted_line[0] == "accept")
		{
			for (std::vector<std::string>::iterator it = splitted_line.begin() + 1; it != splitted_line.end(); ++it)
				lc.accepted_methods.insert(*it);
		}
		else if (splitted_line[0] == "upload_directory")
			lc.upload_directory = splitted_line[1];
		else if (splitted_line[0] == "cgi")
			lc.cgi.insert(std::make_pair(splitted_line[1], splitted_line[2]));
	}
	return lc;
}

void check_several_servers_ports(std::vector<server_config> servers)
{
	std::vector<short> ports;
	for (size_t e = 0; e < servers.size(); e++)
	{
		if (std::find(ports.begin(), ports.end(), servers[e].listen) == ports.end())
			ports.push_back(servers[e].listen);
		else
			std::cout << "ERROR: Sane port in several servers" << std::endl;
	}
}

server_config	parse_server_config(std::fstream *file)
{
	std::string		line;
	server_config	config;

	while (std::getline(*file, line) && line != "}")
	{
		line.resize(line.length() - 1);
		std::vector<std::string>	splitted_line = ws::ft_split(line, " ");

		if (splitted_line[0] == "listen")
			config.listen = std::stoi(splitted_line[1]);
		else if (splitted_line[0] == "server_name")
			config.server_name = splitted_line[1];
		else if (splitted_line[0] == "client_max_body_size")
			config.client_max_body_size = std::stoi(splitted_line[1]);
		else if (splitted_line[0] == "error_page")
		{
			for (std::vector<std::string>::iterator it = splitted_line.begin() + 1; it != splitted_line.end(); ++it)
			{
				if (it == splitted_line.end() - 1)
					config.error_page.second = *it;
				else
					config.error_page.first.push_back(std::stoi(*it));
			}
		}
		else if (splitted_line[0] == "location")
			config.locations.push_back(parse_location(file, splitted_line[1]));
	}
	return config;
}

std::vector<server_config>	parse_config_file(char *file)
{
	std::fstream				config_file;
	std::string					line;
	std::vector<server_config>	servers;
	std::vector<std::string>	allports;

	if (is_valid(file))
	{
		config_file.open(file, std::ifstream::in);

		while (std::getline(config_file, line))
		{
			if (line == "server {") {
				servers.push_back(parse_server_config(&config_file));
			}
		}
		check_several_servers_ports(servers);
		return servers;
	}
	else
	{
		throw (std::invalid_argument("El archivo especificado no es valido o no existe"));
	}
}
