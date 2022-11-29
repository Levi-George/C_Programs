SHELL := bash.exe

CC=gcc
CFLAGS=-g
OBJS=index.o

all: webSearch
webSearch: webSearch.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $< -o $@
%.o : %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm *.o *~ webSearch