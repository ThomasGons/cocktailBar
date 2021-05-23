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
CFLAGS = -g -O2 -Wextra -Wall \
		 -Isrc/libcyaml/include $(shell $(PKGCONFIG) --cflags gtk+-3.0)

MAKE = make

# Target
all: $(EXE)


$(OBJ): $(DSRC)
	@mkdir -p bin
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXE): $(OBJ) $(YAML) 
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY : clean init

clean:
	rm $(OBJ) $(EXE)

init:
	@git submodule init && git submodule update


$(YAML):
	( cd src/libcyaml; $(MAKE) VARIANT=release )
