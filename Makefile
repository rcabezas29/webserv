# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dpuente- <dpuente-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/07 10:50:06 by rcabezas          #+#    #+#              #
#    Updated: 2022/08/22 18:23:40 by dpuente-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

INC = -I includes

CXX = c++ --std=c++98

CXXFLAGS = -Wall -Werror -Wextra -g3 -fsanitize=address

SRCS = main.cpp srcs/networking/Socket.cpp srcs/parse/parse.cpp
SRCS_TEST = Tests/UnitTest.cpp

OBJS = $(SRCS:.cpp=.o)
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

.PHONY: all clean fclean re