# --------------------------------------
# Nombre del archivo ejecutable
# --------------------------------------
NAME = philo

# --------------------------------------
# Directorio de archivos objeto
# --------------------------------------
OBJDIR = obj

# --------------------------------------
# Archivos fuente
# --------------------------------------
SRC = ./src/main.c \
	  ./src/ft_atoi.c

# --------------------------------------
# Archivos objeto
# --------------------------------------
OBJ = $(SRC:src/%.c=$(OBJDIR)/%.o)

# --------------------------------------
# Compilador y opciones
# --------------------------------------
CC = gcc
CFLAGS = -Wall -Wextra -Werror #-pthread -fsanitize=thread



# --------------------------------------
# Colores y estilos
# --------------------------------------
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[0;36m
RESET = \033[0m
BOLD = \033[1m
UNDERLINE = \033[4m

# --------------------------------------
# Regla por defecto
# --------------------------------------
all: $(NAME)

# --------------------------------------
# Regla para compilar el archivo ejecutable
# --------------------------------------
$(NAME): $(OBJ)
	@echo "$(BLUE)Philosophers are thinking...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS) \
		&& echo "$(GREEN)Philosophers have finished thinking!$(RESET)" \
		|| echo "$(RED)An error occurred while philosophers were thinking$(RESET)"

# --------------------------------------
# Regla para compilar archivos .c en archivos .o
# --------------------------------------
$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(dir $@) # Ensure the necessary directory exists
	@echo "$(MAGENTA)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------
# Regla para limpiar archivos objeto
# --------------------------------------
clean:
	@echo "$(YELLOW)Philosophers are clearing their plates...$(RESET)"
	@rm -rf $(OBJDIR)

# --------------------------------------
# Regla para eliminar el ejecutable
# --------------------------------------
fclean: clean
	@echo "$(RED)Philosophers have left the table...$(RESET)"
	@rm -f $(NAME)

# --------------------------------------
# Regla para reconstruir desde cero
# --------------------------------------
re: fclean all

# --------------------------------------
# Declaración de objetivos que no son archivos
# --------------------------------------
.PHONY: all clean fclean re