CC = gcc
CFLAGS = -Wall -g
INCLUDES = -I../include

SRCS = main.c common.c log.c
OBJS = $(SRCS:.c=.o)

MAIN = roma

.PHONY: depend clean

all:    $(MAIN)
	@echo  roma has been compiled

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^
