DEFAULT_TARGET=test
OBJECTS = MY_FIFO.o test.o

CFLAGS = -g -Wall -O3
LDLIBS=
CC=gcc


# Generate application
$(DEFAULT_TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(DEFAULT_TARGET) $(OBJECTS)
	

clean:
	rm -f *.o $(DEFAULT_TARGET)

run:	$(DEFAULT_TARGET)
	./$(DEFAULT_TARGET)
