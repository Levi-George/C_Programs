

CC = gcc
CFLAGS = -gcc

OBJS = index.o


all: webSearch

webSearch: webSearch.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $< -o $@

%.o: %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o *~ webSearch