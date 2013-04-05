CFLAGS   := -O2 -g  -Wall -Wextra 

INCLUDES := ../bit_block_map.h
SOURCES := ../bit_block_map.cpp
BINS := test_bit_block_map test_bloom 

all: $(BINS)  

test_bit_block_map: test_bit_block_map.cpp $(SOURCES)
	g++ $(CFLAGS) $^ -o $@ 
    
test_bloom: test_bloom.cpp $(SOURCES) ../murmur.h ../murmur.c ../bloom_filter.h \
            ../bloom_filter.cpp md5.h md5.c
	g++ $(CFLAGS) $^ -o $@

clean:
	rm -f $(BINS) *.o *.pch 
