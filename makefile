CC=gcc
CFLAGS= -Wall -ggdb -O2
CLEAN_TARGETS=tree tree.o tree.h.gch

tree: tree.o
	${CC} ${CFLAGS} -o tree tree.o tree.h

tree.o: tree.c tree.h
	${CC} ${CFLAGS} -c tree.c tree.h

clean:
	rm ${CLEAN_TARGETS}
