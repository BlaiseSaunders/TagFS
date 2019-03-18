CC = gcc
CFLAGS  = -lm -pthread -Wall -ansi -pedantic -Wextra -g -pg -D_POSIX_C_SOURCE=200112L
CFLAGS += -D_FILE_OFFSET_BITS=64 `mysql_config --cflags`
CFLAGS += -std=c11
LFLAGS  = `mysql_config --libs` `pkg-config fuse --cflags --libs`
DEPS = threads.h
OBJ = main.o threads.o
OUT_EXE = dandyfs


all:$(OUT_EXE)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $<

$(OUT_EXE): $(OBJ)
	$(CC) $(LFLAGS) $^ -o $@


.PHONY:clean install rebuild
clean:
	rm -f *.o $(OUT_EXE)

install:
	cp ./$(OUT_EXE) /bin

rebuild: clean
	 make
