CXX      := clang++
CC       := clang

CXXFLAGS := -std=c++17 -Iinclude -I/opt/homebrew/include
CFLAGS   := -Iinclude -I/opt/homebrew/include

LDFLAGS  := -L/opt/homebrew/lib -lglfw \
            -framework OpenGL \
            -framework Cocoa \
            -framework IOKit \
            -framework CoreVideo

OBJ := main.o glad.o
OUT := app

.PHONY: all run clean

all: $(OUT)

$(OUT): $(OBJ)
	@$(CXX) $(OBJ) $(LDFLAGS) -o $(OUT)

main.o: main.cpp include/shader.h
	@$(CXX) $(CXXFLAGS) -c $< -o $@

glad.o: src/glad.c
	@$(CC) $(CFLAGS) -c $< -o $@

run: all
	@./$(OUT)

clean:
	@rm -f $(OBJ) $(OUT)
