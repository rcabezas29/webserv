/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcabezas <rcabezas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 18:54:09 by dpuente-          #+#    #+#             */
/*   Updated: 2022/08/26 12:22:31 by rcabezas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "webserv.hpp"

namespace	ws
{
	std::vector<std::string>	ft_split(const std::string &s, std::string seperator);	
}
