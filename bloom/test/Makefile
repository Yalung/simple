CFLAGS   := -O2 -g  -Wall -Wextra 

INCLUDES := ../bit_block_map.h
SOURCES := ../bit_block_map.cpp
BINS := test_bit_block_map  

all: $(BINS)  

test_bit_block_map: test_bit_block_map.cpp $(SOURCES)
	g++ $(CFLAGS) $^ -o $@ 
    
clean:
	rm -f $(BINS) *.o *.pch 
