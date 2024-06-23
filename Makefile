
CC = gcc
CFLAGS = 
MAIN_LINK_FLAGS = -L./lib -lcontainers

CONTAINERS_DIR = containers

vpath libcontainers.a lib

main.exe: main.c tokenizer.c parser.c libcontainers.a
	${CC} ${CFLAGS} -o main main.c ${MAIN_LINK_FLAGS}

libcontainers.a: ./lib ${CONTAINERS_DIR}/array.c ${CONTAINERS_DIR}/array.h ${CONTAINERS_DIR}/string.c ${CONTAINERS_DIR}/string.h \
${CONTAINERS_DIR}/tree.c ${CONTAINERS_DIR}/tree.h
	${CC} -o ${CONTAINERS_DIR}/array.o -c ${CONTAINERS_DIR}/array.c
	${CC} -o ${CONTAINERS_DIR}/string.o -c ${CONTAINERS_DIR}/string.c
	${CC} -o ${CONTAINERS_DIR}/tree.o -c ${CONTAINERS_DIR}/tree.c
	ar crsu lib/libcontainers.a ${CONTAINERS_DIR}/array.o ${CONTAINERS_DIR}/string.o ${CONTAINERS_DIR}/tree.o
	${RM} ${CONTAINERS_DIR}/*.o

./lib:
	mkdir lib