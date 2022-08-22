//
// Created by David Puente on 17/8/22.
//

#include "../../libraries/parse.hpp"

int conf_path(char * path)
{
	std::string file_path(path);
	if (file_path.find("conf"))
	{
		std::cout << "Si que esta" << std::endl;
	}
	return (1);
}

void readfile(std::string path)
{
	std::ifstream rfile;
	char * line;

	rfile.open(path);
	if (rfile.is_open())
	{
		while (rfile.getline(line, 200)){
			std::cout << line << std::endl;
		}
		rfile.close();
	}
}