INC = ../../include
CFLAGS = -fPIC -c  -g
LDFLAGS = -L../../lib -lmenu -lhash -lpthread -ldl
#LDFLAGS = -L../../lib -lmenu -lprofile -lhash -lio  -lpthread
OBJS = add.o sub.o
TARGET = libadd.so libsub.so

all: server $(TARGET)

server: server.o
	$(CC) -o $@ $@.o $(LDFLAGS)
	#cp $@ ../../bin/

server.o: server.c
	$(CC) -c -I$(INC) -g $<

libadd.so: add.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/

libsub.so: sub.o 
	$(LD) -shared -o $@ $<
#	cp lib$@.so ../../bin/

.c.o:
	$(CC) $(CFLAGS) -I$(INC) $< 

clean:
	rm *.o lib*.so server 
