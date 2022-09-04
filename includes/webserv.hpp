/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcabezas <rcabezas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 18:19:45 by rcabezas          #+#    #+#             */
/*   Updated: 2022/09/04 11:09:30 by rcabezas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <map>
#include <unistd.h>
#include <sys/socket.h>
#include <poll.h>
#include <netinet/in.h>

#include "server_config.hpp"

std::vector<server_config>	parse_config_file(char *file);
