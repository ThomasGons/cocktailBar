# Makefile COCKTAIL_BAR

# Paths
DSRC = src
DOBJ = obj/cocktail_bar.o
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


$(DOBJ): $(DSRC)/*.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXE): $(DOBJ) $(YAML) 
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm $(DOBJ) $(EXE)

$(YAML):
	pushd libcyaml
	$(MAKE) VARIANT=release
	popd

