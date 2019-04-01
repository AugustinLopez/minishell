# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aulopez <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/02/12 11:06:27 by aulopez           #+#    #+#              #
#    Updated: 2019/04/01 14:04:20 by aulopez          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=minishell

# --- 1.Source/Header ----------------------------------------------------------

SRC=main \
	ms_prompt \
	ms_read \
	ms_split \
	ms_builtin_exit \
	ms_builtin_setenv \
	ms_builtin_unsetenv \
	ms_builtin_echo \
	ms_builtin_cd \
	ms_env_info \
	ms_signal \
	ms_run_cmd \
	ms_builtin_bonus \
	ms_parse_cmd
HDR=libft.h \
	minishell.h

# --- 2.Path -------------------------------------------------------------------

PATH_HDR=includes/
PATH_SRC=srcs/
PATH_OBJ=srcs/obj/
PATH_LIB=libft/

# --- 3.File Variables ---------------------------------------------------------

OBJ_1=$(SRC:%=$(PATH_OBJ)%.o)
INCLUDES=$(HDR:%=$(PATH_HDR)%)

# --- 4.Compilation Variables --------------------------------------------------

CC=gcc
FLAGS=-Wall -Wextra -Werror -g3
OPTION_O=$(CC) $(FLAGS) -c -I$(PATH_HDR)
OPTION_C1=$(CC) $(FLAGS) -o $(NAME) 
OPTION_C2=-I$(PATH_HDR) -L$(PATH_LIB) -lft
RED=\033[31m
GREEN=\033[32m
YELLOW=\033[33m
BLUE=\033[34m
PURPLE=\033[35m
CYAN=\033[36m
EOC=\033[0m

# --- 5.Rules ------------------------------------------------------------------

all: $(NAME)

$(NAME): $(OBJ_1)
	@echo "$(YELLOW)Updating library... $(EOC)"
	@make -C $(PATH_LIB) >> /dev/null
	@echo "$(YELLOW)Compiling $(NAME)...$(EOC)"
	@$(OPTION_C1) $(OBJ_1) $(OPTION_C2)
	@echo "$(GREEN)Done$(EOC)"

$(PATH_OBJ)%.o:$(PATH_SRC)%.c $(INCLUDES)
	@echo "$(CYAN)Creating $@$(EOC)"
	@$(OPTION_O) $< -o $@

onlylib:
	@echo "$(YELLOW)Updating library... $(EOC)"
	@make -C $(PATH_LIB) >> /dev/null
	@echo "$(GREEN)Done$(EOC)"

fcleanlib:
	@echo "$(PURPLE)Removing library... $(EOC)"
	@make -C $(PATH_LIB) fclean > /dev/null

cleanlib:
	@echo "$(BLUE)Cleaning library... $(EOC)"
	@make -C $(PATH_LIB) clean > /dev/null

clean: cleanlib
	@echo "$(BLUE)Deleting object files...$(EOC)"
	@rm -f $(OBJ_1)

fclean: clean fcleanlib
	@echo "$(PURPLE)Deleting $(NAME)...$(EOC)"
	@rm -f $(NAME)
	@echo "$(GREEN)Done$(EOC)"

re: fclean all

.PHONY: all clean fclean re fcleanlib cleanlib onlylib
