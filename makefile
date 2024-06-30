CC = g++
SRCS = src/*.cpp src/**/*.cpp
OUT = bin/wordle
COMPILER_FLAGS = -w -std=c++20
LINKER_FLAGS =
INCLUDE_DIRS =

all: $(SRCS)
	$(CC) $(SRCS) $(INCLUDE_DIRS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OUT)

clean:
	rm ./bin/wordle
