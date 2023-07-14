NAME	=	ircProxy
CC		=	c++
CFLAGS	=	-Wall -Wextra -Werror
SRCS	=	src/main.cpp src/class/Proxy.cpp
IFLAGS	=	-Iinc/
OBJS	=	$(SRCS:.cpp=.o)
RM		=	rm -rf

.cpp.o:
	$(CC) $(CFLAGS) ${IFLAGS} -c $< -o $(<:.cpp=.o)

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) ${IFLAGS} $^ -o $(NAME)

all:		$(NAME)

clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re