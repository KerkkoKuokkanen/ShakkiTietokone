
NAME = CH
TEST_NAME = test_runner
CXX = g++

FLAGS = -Wall -Wextra -Werror -std=c++20 -g

HDR_DIRS = $(shell find hdr -type d)
INCLUDE_FLAGS = $(addprefix -I,$(HDR_DIRS)) -Itests

SRCS = $(shell find srcs -name '*.cpp')
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

TEST_SRCS = $(shell find tests -name '*.cpp')
PROD_SRCS_NO_MAIN = $(filter-out srcs/main.cpp, $(SRCS))
TEST_OBJS = $(TEST_SRCS:.cpp=.o) $(PROD_SRCS_NO_MAIN:.cpp=.o)
TEST_DEPS = $(TEST_OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o $(NAME)

test: $(TEST_OBJS)
	$(CXX) $(FLAGS) $(TEST_OBJS) -o $(TEST_NAME)
	./$(TEST_NAME)

%.o: %.cpp
	$(CXX) $(FLAGS) $(INCLUDE_FLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)
-include $(TEST_DEPS)

clean:
	rm -f $(OBJS) $(DEPS) $(TEST_OBJS) $(TEST_DEPS)

fclean: clean
	rm -f $(NAME) $(TEST_NAME)

re: fclean all

.PHONY: all clean fclean re test
