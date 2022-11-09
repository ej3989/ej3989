INC = ../../include
CFLAGS = -fPIC -c -g
#LDFLAGS = -L../libsrc -lmenu -lhash -lpthread -ldl
#LDFLAGS = -L../../lib -lmenu -lprofile -lhash -lio  -lpthread
OBJS = menu.o profile.o input.o 
TARGET = libmenu.so libarray.so libhash.so libserver.so

all:$(TARGET)

libmenu.so: $(OBJS)
	$(LD) -shared -o $@ $(OBJS)
	cp $@ ../../lib/

libhash.so: hash.o 
	$(LD) -shared -o $@ $< 
	cp $@ ../../lib/

libarray.so: array.o 
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
libserver.so: server.o 
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/

.c.o:
	$(CC) $(CFLAGS) -I$(INC) $< 

clean:
	rm *.o 
	rm *.so 
