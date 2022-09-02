#include "webserv.hpp"
#include "server_config.hpp"
#include "utils.hpp"

// location_config	parse_location(std::fstream *file, std::string path) {
	
// }

server_config	parse_server_config(std::fstream *file) {
	std::string		line;
	server_config	config;

	while (std::getline(*file, line) && line != "}")
	{
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
		// else if (splitted_line[0] == "location")
			// config.locations.push_back();
	}
	return config;
}

std::vector<server_config>	parse_config_file(char *file) {
	std::fstream				config_file;
	std::string					line;
	std::vector<server_config>	servers;

	if (is_valid(file))
	{
		config_file.open(file, std::ifstream::in);

		while (std::getline(config_file, line))
		{
			server_config	serv;
			if (line == "server {")
				serv = parse_server_config(&config_file);
		}
		return servers;
	}
	else{
		//std::cout << "El archivo especificado no es valido o no existe" << std::endl; // TODO: cambiar por por excetion
		throw (std::invalid_argument("El archivo especificado no es valido o no existe"));
		return servers;
	}
}
