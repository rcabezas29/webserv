/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcabezas <rcabezas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 18:54:35 by dpuente-          #+#    #+#             */
/*   Updated: 2022/08/24 16:17:35 by rcabezas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "parse.hpp"
#include "lexer.hpp"
#include "server.hpp"
#include "webserv.hpp"

int	main(int argc, char **argv)
{

	if (argc == 2)
	{
		readfile(argv[1]);
	}
	ws::server	serv;
	serv.connecting();
	return 0;
}
