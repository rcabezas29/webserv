/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpuente- <dpuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 18:54:35 by dpuente-          #+#    #+#             */
/*   Updated: 2022/07/19 18:27:18 by dpuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libraries/utils.hpp"
# include "webserv.hpp"

int	main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    ws::Socket	sock = ws::Socket("Server", AF_INET, SOCK_STREAM, 0, 4242, INADDR_ANY);

    sock.connecting();
    return 0;
}
