compile: atv6.c
	gcc atv6.c -o atv6 -lpthread

run:
	./atv6

clean:
	rm -rf *.o atv6
