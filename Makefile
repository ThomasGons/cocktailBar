# Makefile COCKTAIL_BAR

# Paths
DSRC = $(wildcard src/*.c) $(wildcard src/*.h)
OBJ = bin/cocktail_bar.o
EXE = bin/cocktail_bar

PKGCONFIG = $(shell which pkg-config)

YAML = src/libcyaml/build/release/libcyaml.a 

# compiler
CC = gcc

# Linker flags
LDFLAGS = -lyaml $(shell $(PKGCONFIG) --libs gtk+-3.0)

# Compiler flags
CFLAGS = -g -O2 -Wextra -Wall -Wunused\
		 -Ilibcyaml/include $(shell $(PKGCONFIG) --cflags gtk+-3.0)

# Target
all: $(EXE)


$(OBJ): $(DSRC)
	@mkdir -p bin
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXE): $(OBJ) $(YAML) 
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY : clean

clean:
	rm $(OBJ) $(EXE)

$(YAML):
	pushd libcyaml
	$(MAKE) VARIANT=release
	popd

