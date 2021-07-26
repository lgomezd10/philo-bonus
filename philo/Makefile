# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/26 16:31:50 by lgomez-d          #+#    #+#              #
#    Updated: 2021/07/26 16:31:52 by lgomez-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

RM = rm -rf

CFLAGS	= -Wall -Wextra -Werror -pthread

FILES = main \
	utils \
	load_data \
	philos \
	main_process 

SRCS = ${addsuffix .c, ${addprefix srcs/, ${FILES}}}

OBJS =${SRCS:.c=.o}

${NAME}:	${OBJS}
	gcc -g ${CFLAGS} -o ${NAME} ${OBJS}

all:	${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re:	fclean all