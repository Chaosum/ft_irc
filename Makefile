SRCS							= ${addprefix srcs/, main.cpp Channel.cpp User.cpp Server.cpp Commands.cpp}

OBJS							= ${SRCS:.cpp=.o}

INCLUDES						= ${addprefix inc/, Channel.hpp Server.hpp User.hpp}

NAME							= irc

CC								= c++

CFLAGS							= -Wall -Wextra -Werror -std=c++98

all:			${NAME}

%.o:			%.cpp ${INCLUDES}
						${CC} ${CFLAGS} -c $< -o $@

${NAME}:		${OBJS}
								${CC} ${CFLAGS} -o ${NAME} ${OBJS} 
								@echo compilation complete !

clean:
						@rm -f srcs/*.o inc/*.o
						@echo files cleaned !

fclean:		 clean
						@rm -f ${NAME}

re:					 fclean all

.PHONY: all clean fclean re
