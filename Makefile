LIB_NAME ?= raylib-text-box
LIB_DIR ?= ./lib
BIN_DIR = ./bin
SRC_DIR = ./src


EXTERNAL_LIBRARIES = \
	-L./external/raylib/lib -lraylib \

EXTERNAL_INCLUDES = \
	-I./external/raylib/include \

LOCAL_INCLUDES = \
	-I${SRC_DIR} \

#
#
#

CC = gcc

CFLAGS = -Wall -Wextra -fPIC -g
INCLUDES = $(EXTERNAL_INCLUDES) $(LOCAL_INCLUDES)
LDFLAGS = $(EXTERNAL_LIBRARIES)

SRC = $(wildcard $(SRC_DIR)/*.c)

OBJ_DIR = build
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

STATIC_LIB = $(LIB_DIR)/lib$(LIB_NAME).a
SHARED_LIB = $(LIB_DIR)/lib$(LIB_NAME).so

#
#
#

all: shared static

static: $(OBJS)
	mkdir -p $(LIB_DIR)
	ar rcs $(STATIC_LIB) $(OBJS)

shared: $(OBJS)
	mkdir -p $(LIB_DIR)
	$(CC) -shared -o $(SHARED_LIB) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

examples:
	$(MAKE) -C ./examples MAIN_DIR=$(realpath ./)

clean:
	$(MAKE) clean -C ./examples MAIN_DIR=$(realpath ./)
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR) $(SHARED_LIB) $(STATIC_LIB)

.PHONY: all clean examples
