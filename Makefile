CCFLAGS = -std=c99 -Wall
simulate_cache: simulate_cache.o parse_user_options.o create_cache.o
	gcc -o simulate_cache $(CCFLAGS) simulate_cache.o parse_user_options.o create_cache.o -lm
%.o: %.c parse_user_options.h cache.h
	gcc -c -g $(CCFLAGS) $<

clean:
	rm -f *.o  *~ simulate_cache
