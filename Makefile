NAME = webserver
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g

CORE_DIR = Core
DATA_DIR = Data
OBJS_DIR = objs
UTILS_DIR = utils

CONFIG_SRCS = $(addprefix $(CORE_DIR)/, \
				Config.cpp\
				Validator.cpp\
			)

HTTP_REQUEST_SRCS = $(addprefix $(DATA_DIR)/, \
				Request.cpp \
				Client.cpp \
				Response.cpp \
			)
SOCKET_SRCS = $(addprefix $(CORE_DIR)/, \
			Signal.cpp\
			Socket.cpp\
			ClientSocket.cpp\
			ServerSocket.cpp\
			Epoll.cpp\
			Webserv.cpp\
			)

UTILS_SRC = $(addprefix $(UTILS_DIR)/, \
			getFileContent.cpp \
			number_utils.cpp \
			time.cpp \
			getListDirectory.cpp \
			path_utils.cpp \
		)

SRC = main.cpp
SRC += $(CONFIG_SRCS)
SRC += $(HTTP_REQUEST_SRCS)
SRC += $(SOCKET_SRCS)
SRC += $(UTILS_SRC)

OBJS = $(SRC:%.cpp=$(OBJS_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

reclean: re
	make clean

.PHONY: all clean fclean re reclean