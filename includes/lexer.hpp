/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpuente- <dpuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 16:51:57 by dpuente-          #+#    #+#             */
/*   Updated: 2022/08/23 20:10:02 by dpuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

namespace ft
{
	struct server
	{
		int			*ports;
	};

	struct config
	{
		size_t		file_size;
		int			n_nodes;
		ft::server	node;
	};
}

void			lexer(std::string path);
std::string		*readfile(std::string path);
std::string		*str_split(std::string file);
bool			is_valid(char * line);
int 			lines_file(std::ifstream rfile);

// int input_params(int argc, char **argv);