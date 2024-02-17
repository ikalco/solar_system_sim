# Compiler settings
CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -MD
LDFLAGS = -lSDL2 -lm

# Makefile settings
APPNAME = solar_system_sim
EXT = .c
SRCDIR = src
BUILDDIR = build

# Dirs defined
SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(BUILDDIR)/%.o)

all: $(BUILDDIR)/$(APPNAME)

# Builds the app from objs and links
$(BUILDDIR)/$(APPNAME): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Builds all src into objects
$(BUILDDIR)/%.o: $(SRCDIR)/%$(EXT) | $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILDDIR):
	mkdir -p $@

# Cleans complete project
.PHONY: clean
clean:
	rm $(BUILDDIR)/*

-include $(OBJS:.o=.d)
