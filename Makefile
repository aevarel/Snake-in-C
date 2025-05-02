CC = gcc
FLAGS = -ggdb3 -Wall -Wextra -Wshadow -std=gnu11 -Wno-unused-parameter -Wno-unused-but-set-variable -Werror

# Linking ncurses works differently on Linux and Mac. Detect
# OS to account for this
HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
ifeq ($(SYSTEM),Darwin)
LIBS = -lncurses
FLAGS += -D_XOPEN_SOURCE_EXTENDED
else
LIBS = $(shell ncursesw5-config --libs)
FLAGS += $(shell ncursesw5-config --cflags)
endif

FILES = $(wildcard src/*.c) $(wildcard src/*.h)
OBJS = src/game.o src/game_setup.o src/render.o src/common.o src/linked_list.o src/mbstrings.o src/game_over.o
BINS = snake

# Should address sanitizer be enabled? Default is 1.
# Options are 0 or 1.
ASAN ?= 0
ifeq ($(ASAN),1)
FLAGS += -fsanitize=address
endif

# wildcard rule for compiling object file from source and header
src/%.o: src/%.c $(wildcard src/*.h)
	$(CC) $(FLAGS) -c $< -o $@

# Compile snake game executable
snake: $(OBJS) src/snake.c
	$(CC) $(FLAGS) $^ $(LIBS) -o $@ -lm

# Clean up build files
clean:
	rm -f $(BINS)
	rm -f ${OBJS}
	rm -f test/*.o

# Rebuild everything
rebuild: clean all

# Format the source code using clang-format
format:
	clang-format -style=file -i $(FILES)

.PHONY: all clean format rebuild