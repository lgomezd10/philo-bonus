NAME = philo

RM = rm -rf

CFLAGS	= -Wall -Wextra -Werror -pthread

FILES = main \
	utils \
	str_utils \
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