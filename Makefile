CFLAGS = -Wall


all: pgmnega pgmlimiar pgmmedia pgmmediana


pgmnega: pgmnega.o pgmlib.o commline.o
	gcc $(CFLAGS) pgmnega.o pgmlib.o commline.o -o pgmnega

pgmlimiar: pgmlimiar.o pgmlib.o commline.o
	gcc $(CFLAGS) pgmlimiar.o pgmlib.o commline.o -o pgmlimiar

pgmmedia: pgmmedia.o pgmlib.o commline.o
	gcc $(CFLAGS) pgmmedia.o pgmlib.o commline.o -o pgmmedia

pgmmediana: pgmmediana.o pgmlib.o commline.o
	gcc $(CFLAGS) pgmmediana.o pgmlib.o commline.o -o pgmmediana



pgmnega.o: pgmnega.c pgmlib.h commline.h
	gcc $(CFLAGS) -c pgmnega.c

pgmlimiar.o: pgmlimiar.c pgmlib.h commline.h
	gcc $(CFLAGS) -c pgmlimiar.c

pgmmedia.o: pgmmedia.c pgmlib.h commline.h
	gcc $(CFLAGS) -c pgmmedia.c

pgmmediana.o: pgmmediana.c pgmlib.h commline.h
	gcc $(CFLAGS) -c pgmmediana.c

pgmlib.o: pgmlib.c pgmlib.h commline.h
	gcc $(CFLAGS) -c pgmlib.c

commline.o: commline.c commline.h
	gcc $(CFLAGS) -c commline.c



clean:
	rm -f pgmnega.o pgmlimiar.o pgmmedia.o pgmmediana.o pgmlib.o commline.o *~


purge: clean
	rm -f pgmnega pgmlimiar pgmmedia pgmmediana