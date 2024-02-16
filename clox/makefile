CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
EXEC = main

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@$(MAKE) clean

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS)

run: $(EXEC)
	./$(EXEC)
