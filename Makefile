CC = gcc
CFLAGS = 
MAIN_LINK_FLAGS = -L./lib -lcontainers

CONTAINERS_DIR = containers

vpath libcontainers.a lib

main.exe: main.c libcontainers.a
	${CC} ${CFLAGS} -o main main.c ${MAIN_LINK_FLAGS}

libcontainers.a: ./lib ${CONTAINERS_DIR}/array.c ${CONTAINERS_DIR}/array.h ${CONTAINERS_DIR}/string.c ${CONTAINERS_DIR}/string.h
	${CC} -O3 -o ${CONTAINERS_DIR}/array.o -c ${CONTAINERS_DIR}/array.c
	${CC} -O3 -o ${CONTAINERS_DIR}/string.o -c ${CONTAINERS_DIR}/string.c
	ar rcs lib\libcontainers.a ${CONTAINERS_DIR}\array.o ${CONTAINERS_DIR}\string.o
	del ${CONTAINERS_DIR}\*.o

./lib:
	mkdir lib