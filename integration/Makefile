# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Libraries to link
# LIBS = -lm -lpthread -lcurl -lpaho-mqtt3c -lcjson
# LIBS = -lm -lasound
LIBS = -lm -lpthread -lpaho-mqtt3c -lcjson -lsqlite3 -lasound

# Target application name
TARGET = main_app

# Source files and object files
SRCS = main_thr.c mqtt/mqtt.c mqtt/print.c db/db_9/db_helper.c db/db_9/db_thr.c \
	    sound/fft_thr.c sound/alsa_thr.c sound/sound_freq_last.c \
	   sound/rt_nonfinite.c sound/FFTImplementationCallback.c sound/abs.c

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
	./$(TARGET) 

# Phony targets
.PHONY: all clean run