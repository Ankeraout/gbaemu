CXX=g++ -c
CXXFLAGS=-W -Wall -Wextra -pedantic -Isrc -std=c++17 `sdl2-config --cflags`
LD=g++
LDFLAGS=`sdl2-config --libs`
FASMARM=fasmarm

SOURCES:=$(shell find src/ -type f -name '*.cpp')
OBJECTS=$(SOURCES:src/%.cpp=obj/%.cpp.o)
SUBDIRS=$(dir $(OBJECTS))
EXEC=bin/gbaemu

SOURCES_TESTROMS=$(wildcard testroms/*/*.asm)
BINARY_TESTROMS=$(SOURCES_TESTROMS:testroms/%.asm=testroms/%.gba)

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

all: $(EXEC) testroms

testroms: $(BINARY_TESTROMS)

$(EXEC): bin $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@

obj/%.cpp.o: src/%.cpp $(SUBDIRS)
	$(CXX) $(CXXFLAGS) $< -o $@

testroms/%.gba: testroms/%.asm
	$(FASMARM) $< $@

bin:
	mkdir bin

clean:
	rm -rf bin obj $(BINARY_TESTROMS)
