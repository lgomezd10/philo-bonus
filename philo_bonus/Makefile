# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lgomez-d <lgomez-d@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/26 16:31:26 by lgomez-d          #+#    #+#              #
#    Updated: 2021/07/26 16:31:28 by lgomez-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

RM = rm -rf

CFLAGS	= -Wall -Wextra -Werror -pthread

FILES = main \
	utils \
	load_data \
	philo \
	philo_utils \
	main_process 

SRCS = ${addsuffix .c, ${addprefix srcs/, ${FILES}}}

OBJS =${SRCS:.c=.o}

${NAME}:	${OBJS}
	gcc -g ${CFLAGS} -o ${NAME} ${OBJS}

bonus: ${NAME}

all:	${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re:	fclean all