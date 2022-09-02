#include "webserv.hpp"
#include "server_config.hpp"
#include "utils.hpp"

// barras para paths
// 

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
		return false;
	}
	return true;
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

bool is_valid(char *file)
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
		// std::cout << line << std::endl;
	}
	if(open || semicolon || spaces)
		return false;
	return (true);
}