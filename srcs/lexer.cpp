/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcabezas <rcabezas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 16:52:03 by dpuente-          #+#    #+#             */
/*   Updated: 2022/08/23 20:24:09 by rcabezas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.hpp"

bool is_valid(char *line)
{
	if (!line)
		return (0);
	return (1);
}

int lines_file(std::string path) //TODO: cambiar a utils
{
	std::ifstream rfile;
	rfile.open(path);

	int n = 0;
	std::string line;
	if (rfile.is_open())
	{
		while (getline(rfile, line))
		{
			n++;
		}
	}
	else
		return(-1);
	return (n);
}

std::string *readfile(std::string path)
{
	std::ifstream rfile;
	rfile.open(path);

	int number = lines_file(path);
	std::string line;
	std::string * config_file = NULL;

	if (number > 0)
	{
		config_file = new std::string[number];
		if (rfile.is_open())
		{
			int n = 0;
			while (getline(rfile, line))
			{
				config_file[n] = line;
				n++;
			}
			rfile.close();
		}
	}
	return (config_file);
}

void lexer(std::string path)
{
	std::string * content_file = readfile(path);

	(void)content_file;
}