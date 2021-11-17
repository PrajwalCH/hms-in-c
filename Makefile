DEBUG=0

STD=-std=c99
WARN=-Wall -Wextra -Wpedantic -Wformat=2 -Wwrite-strings -Wno-unused-result

OPTIMIZE=-O2
DEBUG_FLAGS=-g

CC=clang
CFLAGS=$(STD) -MMD

ifeq ($(DEBUG), 1)
	CFLAGS += $(DEBUG_FLAGS) $(WARN) -DDEBUG
else
	CFLAGS += $(OPTIMIZE) -DNDEBUG
endif

BUILD_DIR=./build
INCLUDE_DIR=./include
SRC_DIR=./src

ifneq (,$(wildcard $(INCLUDE_DIR)))
    CFLAGS += -I$(INCLUDE_DIR)
endif

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
DEPS=$(patsubst %.o, %.d, $(OBJS))

TARGET=devin_hotel

LIBS=

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ 

run:
	./build/devin_hotel

-include ($(DEPS))

.PHONY: clean
clean:
	$(RM) $(BUILD_DIR)/*

