
#
# The directory where build artefacts are saved. This is generated on the fly at
# build-time, and is set to be ignored by Git.
#
DIR_BLD = bld

#
# The directory where the library source code kept.
#
DIR_SRC = src

#
# The directory where the test code is kept.
#
DIR_TEST = test

#
# The list of library source files
#
#LIB_SRC = $(sort $(wildcard $(DIR_SRC)/*.c $(DIR_SRC)/core/*.c))
LIB_SRC = $(sort $(shell find $(DIR_SRC)/ -type f -name '*.c'))

#$(shell find flac/ -type f -name '*.flac')
#
# The list of library object files
#
LIB_OBJ = $(patsubst $(DIR_SRC)/%.c, $(DIR_BLD)/%.o, $(LIB_SRC))

#
# The list of test source files.
#
TEST_SRC = $(sort $(wildcard $(DIR_TEST)/*.c))

#
# The test exectuable
#
TEST_BIN = bld/sage-runner


CC = ccache gcc
CFLAGS = -g -Wall -Wextra
LDFLAGS = -lSDL2 -lSDL2_image -lm


$(TEST_BIN): $(LIB_OBJ) $(TEST_SRC)
	$(LINK.c) $^ -o $@

$(DIR_BLD)/%.o: $(DIR_SRC)/%.c | $(DIR_BLD)
	$(COMPILE.c) $^ -o $@

$(DIR_BLD):
	mkdir -p $@ $@/core

all: $(TEST_BIN)

clean:
	rm -rfv $(DIR_BLD)

run: $(TEST_BIN)
	./$(TEST_BIN)

check: $(TEST_BIN)
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all \
		 --track-origins=yes --log-file=$(DIR_BLD)/valgrind.log  \
		 $(TEST_BIN)

.PHONY: all clean run

