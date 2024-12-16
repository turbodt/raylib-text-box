EXTERNAL_DIR ?= $(realpath ../external)
TARGET_DIR ?= $(realpath ../bin)
SRC_DIR = ./src

EXTERNAL_INCLUDES = \
	-I${EXTERNAL_DIR}/raylib/include \
	-I../include \

EXTERNAL_LIBRARIES = \
	-L../lib -lraylib-text-box \
	-L${EXTERNAL_DIR}/raylib/lib -lraylib \

LOCAL_INCLUDES = \
	-I$(SRC_DIR) \

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

TARGET_SOURCES = $(wildcard $(SRC_DIR)/example*.c)
TARGETS = $(patsubst $(SRC_DIR)/example%.c, $(TARGET_DIR)/example%, $(TARGET_SOURCES))

#
#
#

all: $(TARGETS)

$(TARGETS): $(TARGET_DIR)/%: $(OBJ_DIR)/%.o
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGETS)

.PHONY: all clean
