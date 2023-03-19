main:
	gcc solver.c -o solver -O3

test:
	gcc test.c -o test
clean:
	rm solver test