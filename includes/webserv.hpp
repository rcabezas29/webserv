/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpuente- <dpuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 18:19:45 by rcabezas          #+#    #+#             */
/*   Updated: 2022/09/13 19:37:19 by dpuente-         ###   ########.fr       */
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
#include <dirent.h>
#include <poll.h>
#include <netinet/in.h>

#include "server_config.hpp"

// config_file_parser.cpp
std::vector<server_config>		parse_config_file(char *file);
// is_valid.cpp
bool							is_valid(char *file);
bool 							check_paths(std::string path);