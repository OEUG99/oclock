CC = gcc
CFLAGS = $(shell pkg-config --cflags ncurses)
LIBS = $(shell pkg-config --libs ncurses)
TARGET = oclock
SOURCE = oclock.c

$(TARGET): $(SOURCE)
	$(CC) $(SOURCE) $(CFLAGS) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean