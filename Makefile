CC = gcc
LDFLAGS= -lyaml 
CFLAGS = -g -Wall -Wextra -Ilibcyaml/include 
EXEC = cocktail_bar
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
YAML = libcyaml/build/release/libcyaml.a

all: $(EXEC)

cocktail_bar.o: cocktail_bar.c cocktail_bar.h   
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJ) $(YAML) 
	$(CC) -o $@ $^ $(LDFLAGS)
	rm -rf
clean:
	rm -rf *.o

$(YAML):
	pushd libcyaml
	$(MAKE) VARIANT=release
	popd

