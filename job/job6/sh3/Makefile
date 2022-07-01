CC       := gcc
CFLAGS   := -g -Wall -Wno-parentheses
LD       := gcc
LIB      :=

SOURCES := $(wildcard *.c)
OBJECTS := $(subst .c,.o,$(SOURCES))
TARGET  := sh

all: $(OBJECTS) $(LIB)
	$(LD) -o $(TARGET) $(OBJECTS) $(LIB)

%.o: %.c
	@echo CC $<
	@$(CC) $(CFLAGS) -g -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET) tags core

run: all
	./$(TARGET)
