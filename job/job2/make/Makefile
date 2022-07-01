exe:main.o math.o
	cc -o exe main.o math.o

main.o:main.c
	cc -c main.c

math.o:math.c
	cc -c math.c

clean:
	rm -f exe *.o
