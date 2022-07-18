/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpuente- <dpuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 18:54:35 by dpuente-          #+#    #+#             */
/*   Updated: 2022/07/11 16:35:08 by dpuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libraries/utils.hpp"

/* # define AF_INET=
# define SOCK_STREAM */

struct sockaddr_in
{
    __uint8_t         sin_len; 
    sa_family_t       sin_family; 
    in_port_t         sin_port; 
    struct in_addr    sin_addr; 
    char              sin_zero[8];
};

int main(void)
{
    try {
        int sever_fd = socket(/* domain */, /* __cpp_return_type_deduction */);
    }
    catch (char * e) {
        std::cout << e << std::endl;
    }
    return(0);
}   