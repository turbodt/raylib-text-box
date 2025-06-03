LIB_NAME ?= raylib-text-box
LIB_DIR ?= $(realpath ./)/lib
EXTERNAL_DIR ?= $(realpath ./external)
BIN_DIR ?= $(realpath ./)/bin
SRC_DIR = ./src


EXTERNAL_LIBRARIES = \
	-L${EXTERNAL_DIR}/raylib/lib -lraylib \

EXTERNAL_INCLUDES = \
	-I${EXTERNAL_DIR}/raylib/include \

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
ifeq ($(WITHOUT_EFFECTS),1)
    CFLAGS += -DRAYLIB_TEXT_BOX_WITHOUT_EFFECTS
else
    SRC += $(wildcard $(SRC_DIR)/effects/*.c)
endif

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
	mkdir -p $(BIN_DIR)
	$(MAKE) -C ./examples\
		MAIN_DIR=$(realpath ./)\
		TARGET_DIR=$(BIN_DIR)\
		EXTERNAL_DIR=$(EXTERNAL_DIR)

clean:
	$(MAKE) clean -C ./examples MAIN_DIR=$(realpath ./)
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR) $(SHARED_LIB) $(STATIC_LIB)

.PHONY: all clean examples
