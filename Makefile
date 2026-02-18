.PHONY				:	clean fclean re all

NAME				=   stock_exchange
MODE 				?= 	release

#			GCXX

CXX					=	c++
CXXFLAGS			=	-Wall -Wextra -Werror -MMD -MP $(INCLUDE)
LIBS				=	-lSDL2 -lGL

ifeq ($(MODE), debug)
	CXXFLAGS		= 	-Wall -Wextra -MMD -MP $(INCLUDE) -g3 -DDEBUG
endif

#			COMMON

BUILD_DIR			=	.build/
SRC_DIR				=	./src/
INCLUDE 			=	-Iinclude/ -Iinclude/imgui/

#			SRC

SRC_FILES			=	main								\
						imgui/imgui 						\
						imgui/imgui_demo					\
    					imgui/imgui_draw 					\
    					imgui/imgui_tables 					\
    					imgui/imgui_widgets 				\
    					imgui/backends/imgui_impl_sdl2 		\
    					imgui/backends/imgui_impl_opengl3 	\
						classes/video						\

SRC 				= 	$(addprefix $(SRC_DIR), $(addsuffix .cpp, $(SRC_FILES)))
OBJ 				= 	$(addprefix $(BUILD_DIR), $(addsuffix .o, $(SRC_FILES)))
DEPS 				= 	$(addprefix $(BUILD_DIR), $(addsuffix .d, $(SRC_FILES)))

#			RULES

all					:	$(NAME)

$(NAME)				:	$(BUILD_DIR) $(OBJ)
					$(CXX) $(CXXFLAGS) $(OBJ) $(LIBS) -o $(NAME)

$(BUILD_DIR)		:
					mkdir -p $(BUILD_DIR)
					mkdir -p $(BUILD_DIR)/imgui
					mkdir -p $(BUILD_DIR)/imgui/backends
					mkdir -p $(BUILD_DIR)/classes

$(BUILD_DIR)%.o		: 	$(SRC_DIR)%.cpp
					$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean				:
					rm -rf $(BUILD_DIR)

fclean				:	clean
					rm -f $(NAME)
					rm -f imgui.ini

re					:	fclean all