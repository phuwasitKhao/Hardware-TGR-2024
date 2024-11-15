# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -fopenmp

# Libraries to link
LIBS = -lm -lpthread -lpaho-mqtt3c -lcjson -lsqlite3 -lasound -lcurl

# Target application name
TARGET = main_app

# Source files and object files
SRCS = main_thr.c mqtt/mqtt.c mqtt/print.c db/db_9/db_helper.c db/db_9/db_thr_9.c \
       sound/fft_thr.c sound/alsa_thr.c sound/sound_freq_last.c \
       sound/rt_nonfinite.c sound/FFTImplementationCallback.c sound/abs.c \
       rest_api/rest_thr.c db/db_9/db_thr_1.c

# Predictive model source files
MODEL_SRCS = model/AbstractMovingAverage.c model/bsxfun.c model/cepstralCoefficients.c model/ClassificationTree.c \
             model/designMelFilterBank.c model/diff.c model/fft.c model/FFTImplementationCallback.c model/find.c model/handle.c \
             model/ifft.c model/ipermute.c model/mfcc.c model/modwt.c model/modwtphaseshift.c model/MovingRMS.c model/permute.c \
             model/predictML.c model/predictML_data.c model/predictML_emxutil.c model/predictML_initialize.c \
             model/predictML_rtwutil.c model/predictML_terminate.c model/rtGetInf.c model/rtGetNaN.c \
             model/SystemCore.c

# All sources combined
ALL_SRCS = $(SRCS) $(MODEL_SRCS)

# Object files
OBJS = $(ALL_SRCS:.c=.o)

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
