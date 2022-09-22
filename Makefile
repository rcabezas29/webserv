# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rcabezas <rcabezas@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/07 10:50:06 by rcabezas          #+#    #+#              #
#    Updated: 2022/09/22 14:46:00 by rcabezas         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CXX = clang++

CXXFLAGS = -Wall -Werror -Wextra -I includes -g3 -fsanitize=address

SRCS = srcs/webserv.cpp srcs/Socket.cpp srcs/server.cpp srcs/request.cpp \
		srcs/utils.cpp srcs/response.cpp srcs/config_file_parser.cpp \
		srcs/is_valid.cpp srcs/cgi.cpp

OBJS = $(SRCS:.cpp=.o)

SRCS_TEST = Tests/UnitTest.cpp

OBJS_TEST = $(SRCS_TEST:.cpp=.o)

all: $(NAME)

$(NAME):	$(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean:	clean
	rm -f $(NAME)

test:	$(OBJS_TEST)
	$(CXX) $(CXXFLAGS) $(OBJS_TEST) -o $(NAME)

re:	fclean all

fast: re
	./$(NAME) config/webserv.conf

.PHONY: all clean fclean re