# Makefile COCKTAIL_BAR

# Paths
DSRC = src
OBJ = obj/cocktail_bar.o
EXE = cocktail_bar

PKGCONFIG = $(shell which pkg-config)

YAML = $(DSRC)/libcyaml/build/release/libcyaml.a 

# compiler
CC = gcc

# Linker flags
LDFLAGS = -lyaml $(shell $(PKGCONFIG) --libs gtk+-3.0)

# Compiler flags
CFLAGS = -g -O2 -Wextra -Wall -Wunused\
		-Ilibcyaml/include $(shell $(PKGCONFIG) --cflags gtk+-3.0)

# Target
all: $(EXE)


$(OBJ): $(DSRC)/*.c
	@mkdir -p obj
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXE): $(OBJ) $(YAML) 
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm $(OBJ) $(EXE)

$(YAML):
	pushd libcyaml
	$(MAKE) VARIANT=release
	popd

