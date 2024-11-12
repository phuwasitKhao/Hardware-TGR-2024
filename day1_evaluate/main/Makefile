# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -I../dump_mem  # Add -I to include dump_mem directory for headers

# Libraries to link
LIBS = -lm -lpthread -lpaho-mqtt3c -lcjson -lsqlite3

# Target application name
TARGET = main_app

# Source files and object files
SRCS = main.c ../mqtt/mqtt.c ../mqtt/print.c mid_thr.c data_base.c db_helper.c
OBJS = $(SRCS:.c=.o)

# Default target to build the application
all: $(TARGET)

# Rule to link object files and create the executable with libraries
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Run the application
run: $(TARGET)
	./$(TARGET) test

# Phony targets
.PHONY: all clean run
