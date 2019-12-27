#OBJS specifies which files to compile as part of the project
OBJS = src/screen.c src/keyboard.c src/texture.c src/sprite.c src/vector.c \
       src/sprite-factory.c src/entity.c src/entity-factory.c src/arena.c  \
       src/colour.c src/texture-factory.c src/game.c test/runner.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -Wall -Wextra

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lm

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = sage-runner

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

