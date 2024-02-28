# Compiler settings
CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -I./src -MMD -ggdb
LDFLAGS = -lSDL2 -lSDL2_ttf -lm -I./src

# Makefile settings
APPNAME = solar_system_sim
EXT = .c
SRCDIR = src
BUILDDIR = build

# Dirs defined
SRC = $(wildcard $(SRCDIR)/*$(EXT)) $(wildcard $(SRCDIR)/**/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(BUILDDIR)/%.o)

all: $(BUILDDIR)/$(APPNAME)

# Builds the app from objs and links
$(BUILDDIR)/$(APPNAME): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Builds all src into objects
$(BUILDDIR)/%.o: $(SRCDIR)/%$(EXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

# Cleans complete project
.PHONY: clean
clean:
	rm -rf $(BUILDDIR)/*

-include $(OBJ:.o=.d)
