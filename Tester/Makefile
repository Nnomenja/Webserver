# ========================
# Compiler
# ========================
STD = -std=c++17
CXX      = g++
CXXFLAGS = -Wall -Wextra -Werror $(STD) -no-pie

# ========================
# GoogleTest
# ========================
GTEST_DIR = ../googletest
GTEST_INC = $(GTEST_DIR)/googletest/include
GTEST_LIB = $(GTEST_DIR)/build/lib
GTEST_INCLUDES = -I$(GTEST_INC) -I../status

# ========================
# DOCS
# ========================
SRC = \
	./main_test.cpp\
	./HTTP-Request-class/Request.cpp\
	./Ft-read-test-file/readTestFile.cpp\
	./test/Request_test.cpp


OBJ = $(SRC:.cpp=.o)
NAME = gtest

test_init : 
	cd ..; git clone https://github.com/google/googletest.git;\
	cd googletest; mkdir build; cd build; cmake ..; make

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) \
	-L$(GTEST_LIB) -lgtest -lgtest_main -pthread \
	-o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(GTEST_INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
