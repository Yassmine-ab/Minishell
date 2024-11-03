################################################################################
#                                     COLORS                                   #
################################################################################

DEFAULT			= \033[0m
RED				= \033[1;31m
GREEN			= \033[1;32m
YELLOW			= \033[1;33m
MAGENTA			= \033[1;35m
CYAN			= \033[1;36m
WHITE			= \033[1;37m

################################################################################
#                                     CONFIG                                   #
################################################################################

NAME =			minishell
NAME_BONUS =	minishell_bonus
CC =			gcc
CFLAGS =		-Wall -Wextra -Werror
AR =			ar rcs
RM =			rm -f

################################################################################
#                                PROGRAM'S SOURCES                             #
################################################################################

# Directory paths
SRC =			./srcs/
BUILTIN =		$(SRC)builtin/
PARSER =		$(SRC)parser/
UTILS =			$(SRC)utils/
OBJ =			./objs/
MYLIB_DIR =		./my_lib/
INCLUDES =		./includes/ $(MYLIB_DIR)includes/

# Libraries
MYLIB =			-L $(MYLIB_DIR) -l:my_lib.a

# Source files
SRCS =			$(UTILS)utils.c \
				$(SRC)main.c

SRCS_BONUS =	$(SRC)main_bonus.c

# Converts source file paths to object file paths
OBJS =			$(patsubst $(SRC)%, $(OBJ)%, $(SRCS:.c=.o))
OBJS_BONUS =	$(patsubst $(SRC)%, $(OBJ)%, $(SRCS_BONUS:.c=.o))

################################################################################
#                                     RULES                                    #
################################################################################

# Rule for compiling source files into object files
$(OBJ)%.o:		$(SRC)%.c
				@mkdir -p $(dir $@)
				@$(CC) $(CFLAGS) -c $< -o $@ $(foreach dir,$(INCLUDES),-I$(dir))

# Rule for creating the executable
$(NAME):		$(OBJS)
				@make all --no-print-directory -C $(MYLIB_DIR)
				@echo "🔗 $(WHITE)Linking $(YELLOW)$(NAME) $(CYAN)executable $(DEFAULT)..."
				@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
				@echo "$(GREEN)Done $(DEFAULT)✔️"

# Default rule
all:			$(NAME)

# Bonus rule
bonus:			$(OBJS_BONUS)
				@make all --no-print-directory -C $(MYLIB_DIR)
				@echo "🔗 $(WHITE)Linking $(YELLOW)$(NAME_BONUS) $(CYAN)executable $(DEFAULT)..."
				@$(CC) $(CFLAGS) $(OBJS_BONUS) -o $(NAME_BONUS)
				@echo "$(GREEN)Done $(DEFAULT)✔️"

# Rule for cleaning up object files
clean:
				@make clean --no-print-directory -C $(MYLIB_DIR)
				@echo "🧹 $(RED)Cleaning up $(CYAN)project object files $(DEFAULT)..."
				@$(RM) -r $(OBJ)
				@echo "$(GREEN)Done $(DEFAULT)✔️"

# Full clean rule (objects files, executable and libraries)
fclean:
				@make fclean --no-print-directory -C $(MYLIB_DIR)
				@echo "🧹 $(RED)Cleaning up $(CYAN)project object files $(DEFAULT)..."
				@$(RM) -r $(OBJ)
				@echo "$(GREEN)Done $(DEFAULT)✔️"
				@echo "🗑️ $(RED)Deleting $(YELLOW)$(NAME) $(CYAN)executable $(DEFAULT)..."
				@$(RM) $(NAME)
				@echo "$(GREEN)Done $(DEFAULT)✔️"
				@echo "🗑️ $(RED)Deleting $(YELLOW)$(NAME_BONUS) $(CYAN)executable $(DEFAULT)..."
				@$(RM) $(NAME_BONUS)
				@echo "$(GREEN)Done $(DEFAULT)✔️"

# Rebuild rule
re:				fclean all

debug:			fclean
				$(OBJS)
				@echo "🔗 $(WHITE)Linking $(YELLOW)$(NAME) $(CYAN)executable $(DEFAULT)..."
				@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -g3 -fsanitize=address
				@echo "$(GREEN)Done $(DEFAULT)✔️"

help:
				@echo "$(CYAN)all$(DEFAULT)		- Build the executable $(NAME)"
				@echo "$(CYAN)bonus$(DEFAULT)		- Build the executable $(NAME_BONUS)"
				@echo "$(CYAN)clean$(DEFAULT)		- Clean up object files"
				@echo "$(CYAN)fclean$(DEFAULT)		- Clean up all object files and executables"
				@echo "$(CYAN)re$(DEFAULT)		- Rebuild the entire project"
				@echo "$(CYAN)debug$(DEFAULT)		- Run the program with debugging flags -g3 -fsanitize=address"

# Rule to ensure that these targets are always executed as intended, even if there are files with the same name
.PHONY:			all bonus clean fclean re debug help
