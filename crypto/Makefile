CC = gcc
CFLAGS = -Wextra -Werror -pedantic -Wno-deprecated-declarations -I.
LDFLAGS = -lssl -lcrypto
TARGET = libhblk_crypto.a
SOURCES = $(wildcard *.c)
AR = ar
ARFLAGS = rcs
OBJECTS = $(SOURCES:.c=.o)

#Default target
all: $(TARGET)

#Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#Library activation
$(TARGET): $(OBJECTS)
	@$(AR) $(ARFLAGS) $@ $^

#Clean target to remove generated files
clean:
	rm -f $(OBJECTS) $(TARGET)

#Phony targets to prevent conflicts with files named 'all' or 'clean'
.PHONY: all clean
