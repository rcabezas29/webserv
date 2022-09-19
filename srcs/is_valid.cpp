#include "webserv.hpp"
#include "server_config.hpp"
#include "utils.hpp"


bool check_paths(std::string path)
{
	size_t n;

	n = 0;
	if (path[n] != '/')
	{
		if (!(path[n] == '~' && path[n+1] == '/'))
			return false;
	}
	while(n < path.size())
		n++;
	if (path[n-1] != '/')
		return false;
	return true;
}

bool check_spaces(std::string line)
{
	size_t 			n;
	int				spaces;

	spaces = 0;
	n = 0;

	while(n < line.size() && line[n] == ' ')
		n++;
	if (n % 4 == 0)
	{
		if (line[n] == '\n')
			return true;
		while(n < line.size())
		{
			if (line[n] == ' ')
			{
				if (n < line.size() && n > 0 && (line[n-1] == ' ' || line[n+1] == ' ' ))
					return true;
			}
			n++;
		}
		if ((n - 1) >= 0 && line[n-1] == ' ')
			return true;
		if (line == "\n")
			std::cout << "ya ves" << std::endl;
	}
	return false;
}

bool check_semicolon(std::string line)
{
	size_t pos;

	if (line.find("}") != std::string::npos || line.find("{") != std::string::npos)
		return false;
	else
	{

		pos = line.find(";");
		if (pos == std::string::npos)
			return true;
		else if (pos > 0 && pos == (line.size() - 1))
			return false;
			
	}
	return true;
}

bool	root_location(char *file)
{
	std::fstream				config_file;
	std::string					line;
	int							servers;

	servers = 0;
	config_file.open(file, std::ifstream::in);
	while (std::getline(config_file, line))
	{
		if (line == "server {")
			servers++;
		if (line == "    location / {")
		{
			if (servers != 1)
				return true;
			servers--;
		}
	}
	if (servers != 0)
		return true;
	return false;
}

bool	is_valid(char *file)
{
	std::fstream				config_file;
	std::string					line;
	std::vector<server_config>	servers;

	int 						open;
	bool						semicolon;
	bool						spaces;

	config_file.open(file, std::ifstream::in);
	open = 0;
	semicolon = false;
	spaces = false;
	while (std::getline(config_file, line))
	{
		if (line.find("{") != std::string::npos)
			open++;
		if (line.find("}") != std::string::npos)
			open--;
		if (check_semicolon(line))
			semicolon = true;
		if (check_spaces(line))
			spaces = true;
	}
	if(open || semicolon || spaces || root_location(file))
		return false;
	return (true);
}