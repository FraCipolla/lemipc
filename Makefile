NAME=lemipc
SRCDIR=src
OBJDIR=obj
SRC=lemipc.c
SRCS=$(addprefix $(SRCDIR)/, $(SRC))
CC=cc
CFLAGS=-Wall -Wextra -Werror -o3 -Iinclude
OBJ=$(SRC:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "compiling $(NAME)"
	$(CC) $(OBJ) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "cleaning object files"
	@rm -rf $(OBJ)
	@rm -rf $(OBJDIR)

fclean: clean
	@echo "cleaning executable"
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
.SILENT:
