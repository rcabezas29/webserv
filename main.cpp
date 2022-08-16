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


int send_data(int sockfd, const char *msg)
{
	
}

int main(int argc, char **argv)
{
	if (argc >= 2) {
		try {
			int server_fd = socket(PF_INET, SOCK_STREAM, 0);
			printf("%s -- %d", argv[1], server_fd);
		}
		catch (char * e) {
			std::cout << e << std::endl;
		}
	}
	else {
		std::cout << "Not good number of arguments" << std::endl;
	}
    return(0);
}

// int main(int argc, char *argv[])
// {
// 	struct addrinfo hints, *res, *p;
// 	int status;
// 	char ipstr[INET6_ADDRSTRLEN];

// 	if (argc != 2) {
// 		fprintf(stderr,"usage: showip hostname\n");
// 		return 1;
// 	}

// 	memset(&hints, 0, sizeof hints);
// 	hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
// 	hints.ai_socktype = SOCK_STREAM;

// 	if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
// 		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
// 		return 2;
// 	}

// 	printf("IP addresses for %s:\n\n", argv[1]);

// 	for(p = res;p != NULL; p = p->ai_next) {
// 		void *addr;
// 		const char *ipver;

// 		// get the pointer to the address itself,
// 		// different fields in IPv4 and IPv6:
// 		if (p->ai_family == AF_INET) { // IPv4
// 			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
// 			addr = &(ipv4->sin_addr);
// 			ipver = "IPv4";
// 		} else { // IPv6
// 			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
// 			addr = &(ipv6->sin6_addr);
// 			ipver = "IPv6";
// 		}

// 		// convert the IP to a string and print it:
// 		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
// 		printf("  %s: %s\n", ipver, ipstr);
// 	}

// 	freeaddrinfo(res); // free the linked list

// 	return 0;
// }














