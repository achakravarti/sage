SRC = $(sort $(wildcard src/*.c)) $(sort $(wildcard test/*.c))
OBJ = $(patsubst %.c, %.o, $(SRC))

CC = ccache gcc
CFLAGS = -g -Wall -Wextra
LDFLAGS = -lSDL2 -lSDL2_image -lm

EXE = sage-runner

$(EXE): $(OBJ)
	$(LINK.c) $^ -o $@

all: $(EXE)

clean:
	rm -rfv $(EXE) $(OBJ)

run: $(EXE)
	./$(EXE)

.PHONY: all clean run

