CXX=g++ -c
CXXFLAGS=-W -Wall -Wextra -pedantic -Isrc -std=c++17 `sdl2-config --cflags`
LD=g++
LDFLAGS=`sdl2-config --libs`

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
	CXXFLAGS += -DDEBUG -O0 -g
	LDFLAGS += -g
else
	CXXFLAGS += -DRELEASE -O3 -s
	LDFLAGS += -s
endif

CFLAGS += -I`pwd`/src

DUMMY := $(shell mkdir -p $(SUBDIRS))

all: $(EXEC)

$(EXEC): bin $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@

obj/%.cpp.o: src/%.cpp $(SUBDIRS)
	$(CXX) $(CXXFLAGS) $< -o $@

bin:
	mkdir bin

clean:
	rm -rf bin obj
