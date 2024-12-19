# Makefile to ALGAE-dev

test: unitTests.c test.c algae.h 
	make -C ../gcollect/ gcd
	gcc -O0 -g -DWIN32 -o tests.exe	../gcollect/gcd.o   unitTests.c test.c
	

clean:
	rm -f tests.exe
	make -C ../gcollect/ clean

