TARGET := sysinfo 

BUILD_DIR := build/
SRCS_DIR := src/
VPATH = $(SRCS_DIR)

SRCS := $(notdir $(wildcard src/*.c))
OBJECTS := $(SRCS:%.c=%.o)
CC = gcc
CFLAGS = -Wall -Wextra -I ./include

.PHONY : all
all : $(OBJECTS) $(TARGET)

$(OBJECTS): %.o : %.c | $(BUILD_DIR)$(SRCS_DIR)	
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)$(SRCS_DIR)$@

$(BUILD_DIR)$(SRCS_DIR):
	mkdir -p $(BUILD_DIR)$(SRCS_DIR)

$(TARGET): $(OBJECTS)
	$(CC) $(addprefix $(BUILD_DIR)$(SRCS_DIR), $^) $(CFLAGS) -o $(TARGET)	
 	
.PHONY : clean
clean :
		rm -r $(BUILD_DIR) $(TARGET)

.PHONY : debug
debug : CFLAGS += -g -O0
debug : $(OBJECTS) $(TARGET)


