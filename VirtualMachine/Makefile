CC=gcc

SOURCE= ./src/
INCLUDE= ./include/
BUILD=./build/

CFLAGS= -std=c11 -Wall -Wextra -Wshadow -I$(INCLUDE)

all: $(BUILD)run.o $(BUILD)memory.o $(BUILD)cpu.o $(BUILD)loader.o
	$(CC) $(CFLAGS) $^ -o $(BUILD)$@
	rm -r $(BUILD)*o
$(BUILD)run.o: $(SOURCE)run.c 
	$(CC) $(CFLAGS) -c  $<  -o $@ 
$(BUILD)memory.o: $(SOURCE)memory.c 
	$(CC) $(CFLAGS) -c  $<  -o $@ 
$(BUILD)cpu.o: $(SOURCE)cpu.c
	$(CC) $(CFLAGS) -c  $^  -o $@
$(BUILD)loader.o: $(SOURCE)loader.c
	$(CC) $(CFLAGS) -c  $^  -o $@

clear: 
	rm -r $(BUILD)*.o > /dev/null
clearall:
	rm -r $(BUILD)*.o > /dev/null
	rm -r $(BUILD)all

% : %.c 
	$(CC) $(CFLAGS)  $(SOURCE)/$<  -o $@ 