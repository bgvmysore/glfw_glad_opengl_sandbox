CC=gcc

INCLUDE_DIRS=-Iexternal/glad/include -Iexternal/stb
CFLAGS=-Wall -O0 -g $(INCLUDE_DIRS)

LDFLAGS=-lglfw -lGL -ldl -lm

SRC_FILES=main.c external/glad/src/glad.c shader_loader.c

TARGET_NAME=triangle.out

$(TARGET_NAME): $(SRC_FILES)
	$(CC) $(CFLAGS) -o$@ $^ $(LDFLAGS)

clean:
	rm -rf *.out
