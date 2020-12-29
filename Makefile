CC = gcc -c
CFLAGS = -W -Wall -Wextra -pedantic -Isrc -std=c11 `sdl2-config --cflags`
LD = gcc
LDFLAGS = `sdl2-config --libs`
FASMARM = fasmarm

CORE_SOURCES = \
	$(wildcard src/core/*.c)

SOURCES = \
	$(CORE_SOURCES) \
	src/gbaemu.c \
	src/io.c \
	src/frontend/sdl2.c

TEST_SOURCES = \
	test/main.c \
	test/libtest.c \
	test/test_dummy.c \
	src/frontend/dummy.c

OBJECTS = $(SOURCES:%.c=%.c.o)
CORE_OBJECTS = $(CORE_SOURCES:%.c=%.c.o)
TEST_OBJECTS = $(TEST_SOURCES:%.c=%.c.o)
SUBDIRS = $(dir $(OBJECTS))
EXEC = bin/gbaemu
TEST_EXEC = bin/test

SOURCES_TESTROMS = $(wildcard testroms/*/*.asm)
BINARY_TESTROMS = $(SOURCES_TESTROMS:testroms/%.asm=testroms/%.gba)

ifeq ($(MODE),)
	MODE = release
endif

ifeq ($(OS),Windows_NT)
	EXEC := $(EXEC).exe
	TEST_EXEC := $(TEST_EXEC).exe
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

all: $(EXEC) testroms

testroms: $(BINARY_TESTROMS)

$(EXEC): bin $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@

%.c.o: %.c $(SUBDIRS)
	$(CC) $(CFLAGS) $< -o $@

testroms/%.gba: testroms/%.asm
	$(FASMARM) $< $@

bin:
	mkdir bin

$(TEST_EXEC): bin $(CORE_OBJECTS) $(TEST_OBJECTS)
	$(LD) $(CORE_OBJECTS) $(TEST_OBJECTS) -o $@

test: $(TEST_EXEC)
	$(TEST_EXEC)

clean:
	rm -rf bin $(BINARY_TESTROMS) $(OBJECTS) $(TEST_OBJECTS)

.PHONY: test all testroms

