/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcabezas <rcabezas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 18:54:09 by dpuente-          #+#    #+#             */
/*   Updated: 2022/09/16 11:39:33 by rcabezas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "webserv.hpp"

namespace	ws
{
	std::vector<std::string>	ft_split(const std::string &s, std::string seperator);
	
	bool						is_error_code(short code);
	std::string					generate_reason_phrase(short code);
	std::string					file_to_text(std::fstream &file);
	bool						check_if_dir(std::string path);
	int							matrix_length(char **m);
}
