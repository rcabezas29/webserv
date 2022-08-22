/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpuente- <dpuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 18:54:35 by dpuente-          #+#    #+#             */
/*   Updated: 2022/08/22 18:45:21 by dpuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libraries/utils.hpp"
#include "libraries/parse.hpp"
#include "webserv.hpp"

int	main(int argc, char **argv)
{

	if (argc == 2)
	{
		readfile(argv[1]);
	}

	
    // ws::Socket	sock = ws::Socket("Server", AF_INET, SOCK_STREAM, 0, 4242, INADDR_ANY);

    // sock.connecting();
    return 0;
}
