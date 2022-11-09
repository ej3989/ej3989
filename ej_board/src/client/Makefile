OBJS = client.o
SRCS = $(OBJS:.o=.c)
LDFLAGS=-L../../lib -lpthread -larray -ldl
INC=-I../../include
CFLAGS= -g $(INC)

client:$(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) 
	cp $@ ../../bin/

.c.o:$(SRCS)
	$(CC) -c $< $(CFLAGS)

clean:
	rm *.o client 
