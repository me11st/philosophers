# Name of the executable
NAME = philo

# Directories
OBJ_DIR = obj

# Libraries and compiler
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -f

# Source and object files
SRC_FILES = main philos
SRC = $(addsuffix .c, $(addprefix src/, $(SRC_FILES)))
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

# Include directories
INCLUDES = -Iinclude

# Default target
all: $(OBJ_DIR) $(NAME)

# Compiling object files
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Linking the executable
$(NAME): $(OBJ)
	@echo "Linking the executable..."
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(NAME) is ready."

# Create the object directory
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Clean object files
clean:
	@$(RM) -r $(OBJ_DIR)
	@echo "Object files cleaned."

# Clean object files and executable
fclean: clean
	@$(RM) $(NAME)
	@echo "$(NAME) and related files cleaned."

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re