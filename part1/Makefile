# The variable CC specifies which compiler will be used.
# (because different unix systems may use different compilers)
CC=gcc

# The variable CFLAGS specifies compiler options
#   -c :    Only compile (don't link)
#   -Wall:  Enable all warnings about lazy / dangerous C programming 
#   -std=c99: Using newer C99 version of C programming language
CFLAGS=-c -Wall -std=c99 -g

# All of the .h header files to use as dependencies
HEADERS=helper_functions.h your_functions.h

# All of the object files to produce as intermediary work
OBJECTS=sorting.o helper_functions.o your_functions.o

# The final program to build
EXECUTABLE=sorting_program

# --------------------------------------------

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o $(EXECUTABLE)
