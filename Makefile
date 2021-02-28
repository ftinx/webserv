GREEN = \033[0;32m
RED = \033[0;31m
LIGHT_SKYBLUE_COLOR=\033[36m\033[01m
CLEAR = \033[0m

NAME = Webserv
CC = clang++ -std=c++98
FLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address

DIR_HEADER =	./includes/
DIR_SRC =		./srcs/

SRC =	main.cpp \
		Server.cpp \
		Response.cpp \

SRCS = $(addprefix $(DIR_SRC), $(SRC))
OBJS = $(SRCS:%.cpp=%.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) -I $(DIR_HEADER) $(OBJS) -o $(NAME)
	@echo "$(LIGHT_SKYBLUE_COLOR)MAKEFILE DONE$(CLEAR)"

%.o: %.cpp
	@$(CC) $(FLAGS) -I $(DIR_HEADER) -c $< -o $@
	@echo "$(GREEN)Compiled "$<" successfully!$(CLEAR)"

clean:
	@echo "$(RED)rm $(OBJS)$(CLEAR)"
	@rm -f $(OBJS)
	@echo "$(GREEN)clean successfully!$(CLEAR)"

fclean: clean
	@echo "$(RED)rm $(NAME)$(CLEAR)"
	@rm -f $(NAME)
	@echo "$(GREEN)fclean successfully!$(CLEAR)"

re: fclean all

.PHONY: all clean fclean re