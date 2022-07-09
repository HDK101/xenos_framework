CC=gcc
CFLAGS=-Wfatal-errors -Wall -Wextra -Wconversion -Wshadow
UNUSED_CFLAGS=-Wpedantic
LIBS=-lm -lallegro_font -lallegro_primitives -lallegro_acodec -lallegro_audio -lallegro_image -lallegro
SOURCES=$(wildcard src/common/*.c) $(wildcard src/common/**/*.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))
EXE=bin/main

%.o: %.c  
	$(CC) $(CFLAGS) -g -c $< -o $@

all: $(EXE) 

$(EXE): $(OBJECTS) 
	$(CC) $(OBJECTS) -g -o $(EXE) $(CFLAGS) $(LIBS)

clean:
	@find . -name '*.o' -delete
	@find . -name '*.c~' -delete

format:
	@indent -bad -nbap -nsob -br -cli2 -ce -cli4 -nbc -bbo -npcs -ncs -i4 -lp -npsl -lps -nut src/**/*.c
