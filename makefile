
NAME = CH
CXX = g++

FLAGS = -Wall -Wextra -Werror -std=c++20 -g -fsanitize=address

HDR_DIRS = $(shell find hdr -type d)
INCLUDE_FLAGS = $(addprefix -I,$(HDR_DIRS))

SRCS = $(shell find srcs -name '*.cpp')
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(FLAGS) $(INCLUDE_FLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
