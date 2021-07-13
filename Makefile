# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: besellem <besellem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/25 01:10:15 by besellem          #+#    #+#              #
#    Updated: 2021/07/13 18:15:40 by besellem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	pipex

## Main directories
BUILD_DIR	:=	.build
SRCS_DIR	:=	srcs
INCS		:=	-I incs -I libft/incs
LIBFLAGS	:=	-L./libft -lft
LIB_DIR		:=	libft
SUB_DIR		:=	srcs
OBJ_DIR 	:=	$(BUILD_DIR)/obj
DIRS		:=	$(OBJ_DIR) $(addprefix $(OBJ_DIR)/, $(SUB_DIR))


## Main srcs
SRCS		:=	init_pipex.c \
				memory_management.c \
				pipex.c \
				utils.c


## Objects
OBJS 		:=	$(SRCS:%.c=$(OBJ_DIR)/%.o)


## Commands
CC			:=	clang
CFLAGS		:=	-Wall -Wextra -Werror -g3 -fsanitize=address
BONUS_FLAG	:=
RM			:=	rm -f


# Colors
CLR_COLOR	:= \033[0m
RED			:= \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
CYAN 		:= \033[1;36m


## Compilation
$(NAME):	$(OBJS)
			@echo "Compiling $(RED)Libft $(CLR_COLOR)..."
			@$(MAKE) -C $(LIB_DIR) >/dev/null
			@echo "Creating $(RED)$@ $(CLR_COLOR)..."
			@$(CC) $(CFLAGS) $(INCS) $^ -o $@ $(LIBFLAGS)
			@echo "$(GREEN)Compilation $(YELLOW)of $(RED)$@ $(BLUE)done$(CLR_COLOR)"

all:		$(NAME)

bonus:		BONUS_FLAG := -DBONUS
bonus:		all

clean:
			@ echo "Deleting $(CYAN)pipex $(CLR_COLOR)objs ..."
			@ $(RM) $(OBJS)
			@ rm -rf $(BUILD_DIR)

fclean:		clean
			@echo "Deleting $(CYAN)libft $(CLR_COLOR) ..."
			@$(MAKE) fclean -C $(LIB_DIR) >/dev/null
			@ echo "Deleting $(CYAN)pipex $(CLR_COLOR)binary ..."
			@ $(RM) $(NAME)

re:			fclean all

$(BUILD_DIR):
			@ echo "Creating $(RED)$@ $(CLR_COLOR)..."
			@ mkdir $@ $(DIRS)

$(OBJ_DIR)/%.o:$(SRCS_DIR)/%.c ./incs/pipex.h | $(BUILD_DIR)
			@ echo "Compiling $(YELLOW)$< $(CLR_COLOR)..."
			@ $(CC) $(CFLAGS) -c $< -o $@ $(INCS) $(BONUS_FLAG)

.PHONY:		all bonus clean fclean re
