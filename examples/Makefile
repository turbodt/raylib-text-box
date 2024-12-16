TARGET_DIR = ../bin
SRC_DIR = ./src

EXTERNAL_INCLUDES = \
	-I../external/raylib/include \
	-I../include \

EXTERNAL_LIBRARIES = \
	-L../lib -lraylib-text-box \
	-L../external/raylib/lib -lraylib \

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

$(TARGETS): $(OBJS) $(TARGET_DIR)
	$(CC) $(OBJS) $(INCLUDES) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
