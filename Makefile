CC=gcc

INCLUDE_DIRS=-Iexternal/glad/include -Iexternal/stb -Iexternal/cglm/include
CFLAGS=-Wall -O0 -g $(INCLUDE_DIRS)

LD_DIRS= -Llibs/DEP_CGLM
LDFLAGS=-lglfw -lGL -ldl -lm $(LD_DIRS) -lcglm

SRC_FILES=main.c external/glad/src/glad.c shader_loader.c texture_loader.c

TARGET_NAME=triangle.out

$(TARGET_NAME): $(SRC_FILES)
	$(CC) $(CFLAGS) -o$@ $^ $(LDFLAGS)

clean:
	rm -rf *.out

clean_deps:
	rm -rf libs/DEP_*

libs/DEP_CGLM/libcglm.a: libs/DEP_CGLM external/cglm/CMakeLists.txt
	cmake -S external/cglm -B $< -DCGLM_SHARED=OFF -DCGLM_STATIC=ON
	cmake --build $<

libs/DEP_CGLM:
	mkdir -v $@

deps: libs/DEP_CGLM/libcglm.a
