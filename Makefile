PROGRAMS+=readline-heap
PROGRAMS+=linkedlist-lab
PROGRAMS+=sort-strings

OBJS=malloc.o os-interface.o

CFLAGS += -Wall

all: ${PROGRAMS}

readline-heap: test-programs/readline-heap.c ${OBJS}
	${CC} ${CFLAGS} ${LIBS} -o "$@" test-programs/readline-heap.c ${OBJS}
linkedlist-lab: test-programs/linkedlist-lab.c ${OBJS}
	${CC} ${CFLAGS} ${LIBS} -o "$@" test-programs/linkedlist-lab.c ${OBJS}
sort-strings: test-programs/sort-strings.c ${OBJS}
	${CC} ${CFLAGS} ${LIBS} -o "$@" test-programs/sort-strings.c ${OBJS}

malloc.o: malloc.c os-interface.h
os-interface.o: os-interface.c os-interface.h

clean:
	rm -f ${PROGRAMS} *.o test-programs/*.o *.core
