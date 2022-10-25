CC=gcc

INCLUDE_DIRS=-Iexternal/glad/include
CFLAGS=-Wall -O0 -g $(INCLUDE_DIRS)

LDFLAGS=-lglfw -lGL -ldl

SRC_FILES=main.c external/glad/src/glad.c

TARGET_NAME=triangle.out

$(TARGET_NAME): $(SRC_FILES)
	$(CC) $(CFLAGS) -o$@ $^ $(LDFLAGS)

clean:
	rm -rf *.out
