CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude

SRC = main.c $(wildcard src/*.c)

OBJ = $(SRC:.c=.o)

TARGET = bin/app

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
