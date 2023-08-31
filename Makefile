MAKEFLAGS += --no-builtin-rules

MKDIR := mkdir -p
RM := rm -rf
CC := gcc -c
LD := gcc

CFLAGS += -MMD -MP
CFLAGS += -W -Wall -Wextra
CFLAGS += -std=gnu99 -pedantic-errors
CFLAGS += -g3 -O0
CFLAGS += -Iinclude
CFLAGS += `sdl2-config --cflags`
CFLAGS += -mwindows
LDFLAGS += `sdl2-config --libs`

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SOURCES := $(call rwildcard, ., *.c)
OBJECTS := $(patsubst ./%.c, obj/%.c.o, $(SOURCES))
DIRECTORIES := $(patsubst ./%, obj/%, $(dir $(SOURCES)))
EXECUTABLE := bin/gbaemu
DEPENDENCIES := $(patsubst ./%.c, obj/%.c.d, $(SOURCES))

all: dirs $(EXECUTABLE)

obj/%.c.o: ./%.c
	$(CC) $(CFLAGS) $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	$(RM) bin obj

-include $(DEPENDENCIES)

dirs:
	$(MKDIR) bin $(DIRECTORIES)

.PHONY: all clean dirs
