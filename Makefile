NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g

CORE_DIR = Core
DATA_DIR = Data
OBJS_DIR = objs
HTTP_DIR = HTTP
UTILS_DIR = utils
EXCEPTION_DIR = Exception
REQUEST_TMP_DIR = .tmp

UTILS_SRCS = $(addprefix $(UTILS_DIR)/, \
			getFileContent.cpp \
			number_utils.cpp \
			time.cpp \
			getListDirectory.cpp \
			path_utils.cpp \
			PathUtils.cpp \
			Encoding.cpp \
		)

NETWORK_SRCS = $(addprefix $(CORE_DIR)/Network/, \
				Epoll.cpp \
				ServerSocket.cpp \
				Signal.cpp \
				Socket.cpp \
			)
REQUEST_SRCS = $(addprefix $(CORE_DIR)/$(HTTP_DIR)/Request/, \
				HttpRequestParser.cpp \
				RequestProcessor.cpp \
			)

SETTINGS_SRCS = $(addprefix $(CORE_DIR)/Settings/, \
				Config.cpp \
				Validator.cpp \
				File.cpp \
				helpers.cpp \
			)

DATA_SRCS = $(addprefix $(DATA_DIR)/, \
				Client.cpp \
				Request.cpp \
				Response.cpp \
				Process.cpp \
				RequestLogger.cpp \
			)

EXCEPTION_SRC = $(addprefix $(EXCEPTION_DIR)/, \
				ServerException.cpp  \
			)

REQUEST_PROCESS_STRATEGY = $(addprefix $(CORE_DIR)/$(HTTP_DIR)/Request/RequestProcessStrategy/, \
				DynamicStrategy.cpp \
				Redirectionrategy.cpp \
				StaticStrategy.cpp \
				DirectoryStrategy.cpp \
				UploadStrategy.cpp \
			)

REQUEST_PARSE_STATE = $(addprefix $(CORE_DIR)/$(HTTP_DIR)/Request/RequestParserState/, \
				ARequestParserState.cpp \
				MethodParser.cpp \
				UriParser.cpp \
				HttpVersionParser.cpp \
				HeaderParser.cpp \
 				BodyParser.cpp \
				MetadataParser.cpp\
			)

HTTP_SRCS = $(addprefix $(CORE_DIR)/$(HTTP_DIR)/, \
				Webserv.cpp \
				ErrorProcess.cpp \
				MimeTypes.cpp \
				Template.cpp \
				CgiParser.cpp \
			)

SRC = main.cpp
SRC += $(SETTINGS_SRCS)
SRC += $(REQUEST_SRCS)
SRC += $(NETWORK_SRCS)
SRC += $(UTILS_SRCS)
SRC += $(DATA_SRCS)
SRC += $(EXCEPTION_SRC)
SRC += $(REQUEST_PROCESS_STRATEGY)
SRC += $(REQUEST_PARSE_STATE)
SRC += $(HTTP_SRCS)

OBJS = $(SRC:%.cpp=$(OBJS_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	@mkdir -p $(REQUEST_TMP_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)
	rm -rf $(REQUEST_TMP_DIR)

re: fclean all

reclean: re
	make clean

.PHONY: all clean fclean re reclean