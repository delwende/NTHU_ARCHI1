# Makefile for project 1

CC = cc
CFLAGS = -c -Wall
LDFLAGS = 
SOURCES = simulator.c execute.c operation.c 
OBJECTS = $(SOURCES:.c = .o)
EXECUTABLE = single_cycle

# Include .h file here
INCLUDES =  simulator.h operation.h
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
%.o: %.c ${INCLUDES}
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE) *.bin *.rpt
