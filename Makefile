CC = gcc -c
CFLAGS = -W -Wall -Wextra -pedantic -Isrc -std=c99 `sdl2-config --cflags`
LD = gcc
LDFLAGS = `sdl2-config --libs`

CORE_SOURCES = \
	$(wildcard src/core/*.c)

SOURCES = \
	$(CORE_SOURCES) \
	src/gbaemu.c \
	src/io.c \
	src/frontend/sdl2.c

OBJECTS = $(SOURCES:%.c=%.c.o)
CORE_OBJECTS = $(CORE_SOURCES:%.c=%.c.o)
SUBDIRS = $(dir $(OBJECTS))
EXEC = bin/gbaemu

ifeq ($(MODE),)
	MODE = release
endif

ifeq ($(OS),Windows_NT)
	EXEC := $(EXEC).exe
endif

ifeq ($(MODE), debug)
	CFLAGS += -DDEBUG -O0 -g
	LDFLAGS += -g
else
	CFLAGS += -DRELEASE -O3 -s
	LDFLAGS += -s
endif

CFLAGS += -I`pwd`/src

DUMMY := $(shell mkdir -p $(SUBDIRS))

all: $(EXEC)

$(EXEC): bin $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@

%.c.o: %.c $(SUBDIRS)
	$(CC) $(CFLAGS) $< -o $@

bin:
	mkdir bin

clean:
	rm -rf bin $(OBJECTS)

.PHONY: all

