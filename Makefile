SRCS	= philo.c ft_philoutils.c ft_utils.c ft_parsearg.c ft_listutils.c

OBJS	= ${SRCS:.c=.o}

SRCSB	=

OBJB	= ${SRCSB:.c=.o}

NAME	= philo

CC	= gcc

RM	= rm -f

CFLAGS	= -Wall -Wextra -Werror #-g3 -fsanitize=address

PTHREADFLAGS = -lpthread 

.c.o:
		${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}:	${OBJS}
				${CC} ${CFLAGS} -o $(NAME) $(OBJS) $(PTHREADFLAGS) 

bonus:		all ${OBJB}
				@make OBJS="${OBJB}"

all:		${NAME}

clean:
				${RM} ${OBJS} ${OBJB}

fclean: 	clean
				${RM} ${NAME}

re: 		fclean all

rebonus:	fclean bonus

.PHONY:		all bonus clean fclean re rebonus
