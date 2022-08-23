/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcabezas <rcabezas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 18:54:35 by dpuente-          #+#    #+#             */
/*   Updated: 2022/08/23 20:22:51 by rcabezas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "parse.hpp"
#include "lexer.hpp"
#include "webserv.hpp"

int	main(int argc, char **argv)
{

	if (argc == 2)
	{
		readfile(argv[1]);
	}

	
    ws::Socket	sock = ws::Socket("Server", AF_INET, SOCK_STREAM, 0, 4242, INADDR_ANY);

	sock.connecting();

	return 0;
}
