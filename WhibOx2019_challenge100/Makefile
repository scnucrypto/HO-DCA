CC=gcc-9
CFLAGS=-w -mcmodel=large
OBJS=100.o main.o subfunc.o f.o end_circuit.o

attack: ${OBJS}
	${CC} ${CFLAGS} $^ -o $@
