# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Libraries to link
LIBS = -lm -lasound

# Target application name
TARGET = sound_app

# Source files and object files
# SRCS = sound_app.c abs.c alsa_thr.c FFTImplementationCallback.c fft_thr.c rtGetInf.c rtGetNaN.c rt_nonfinite.c sound_freq.c sound_freq_initialize.c sound_freq_terminate.c
SRCS = sound_app.c fft_thr.c alsa_thr.c sound_freq_last.c rt_nonfinite.c FFTImplementationCallback.c abs.c
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