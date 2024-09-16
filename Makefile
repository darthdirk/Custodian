# Variables
CC=gcc
CFLAGS=-g -Wall
SRC=src
OBJ=obj
PLUGINS=plugins
DIST=dist
BIN=custodian


SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

#PLUGIN_SRCS:=$(wildcard $(SRC)/$(PLUGINS)/*.c)
#PLUGIN_OBJS:=$(patsubst $(SRC)/$(PLUGINS)/%.c, $(DIST)/$(PLUGINS))

all: $(DIST)/$(BIN) $(PLUGINS)

# build the final binary -ldl link dynamic lib
$(DIST)/$(BIN): $(DIST) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@
#-ldl $(LIBSSL)

# compile my .c files to .o (make sure that the obj dir has been created)
#$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h $(OBJ)
#	$(CC) $(CFLAGS) -I$(SRC) -c $< -o $@

# four modules that don't have matching header files
# right now, that's just main.c
$(OBJ)/%.o: $(SRC)/%.c $(OBJ)
	$(CC) $(CFLAGS) -I$(SRC) -c $< -o $@

$(PLUGINS): $(DIST) $(DIST)/$(PLUGINS) $(PLUGIN_OBJS)

#this one need to be fixed
#$(DIST)/$(PLUGINS)/%.$(LIBEXT): $(SRC)/$(PLUGINS)/%.c $(SRC)
#	$(CC) $(CFLAGS) $(LIBFLAGS) $< $(OBJ)/hashing.o -o $@

$(OBJ):
	mkdir $@

$(DIST):
	mkdir $@

$(DIST)/$(PLUGINS):
	mkdir $@

clean:
	rm -rf $(OBJ)
	rm -rf $(DIST)