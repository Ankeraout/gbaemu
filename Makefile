CC=c++ -c
CFLAGS=-W -Wall -Wextra -pedantic -std=gnu++98
LD=c++
LDFLAGS=

SOURCES:=$(shell find src/ -type f -name '*.cpp')
OBJECTS=$(SOURCES:src/%.cpp=obj/%.cpp.o)
SUBDIRS=$(dir $(OBJECTS))
EXEC=bin/gbaemu

ifeq ($(MODE),)
	MODE = release
endif

ifeq ($(OS),Windows_NT)
	EXEC := $(EXEC).exe
endif

ifeq ($(MODE), debug)
	CFLAGS += -DDEBUG -O0 -g
	LDFLAGS += -DDEBUG -O0 -g
else
	CFLAGS += -DRELEASE -O3 -s
	LDFLAGS += -DRELEASE -O3 -s
endif

CFLAGS += -I`pwd`/src

DUMMY := $(shell mkdir -p $(SUBDIRS))

all: $(EXEC)

$(EXEC): bin $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@

obj/%.cpp.o: src/%.cpp $(SUBDIRS)
	$(CC) $(CFLAGS) $< -o $@

bin:
	mkdir bin

clean:
	rm -rf bin obj
