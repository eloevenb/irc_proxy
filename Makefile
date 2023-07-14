NAME	=	ircProxy
CC		=	c++
CFLAGS	=	-Wall -Wextra -Werror
SRCS	=	
IFLAGS	=	-Iinc/
OBJS	=	$(SRCS:.cpp=.o)
RM		=	rm -rf

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $(<:.cpp=.o)

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $^ -o $(NAME)

all:		$(NAME)

clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re