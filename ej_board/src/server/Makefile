.SUFFIXES:.c.o
OBJS=main.o 
#SRCS=main.c array.c srever.c
SRCS=$(OBJS:.o=.c)
#OBJS=$(foreach ../,../src,./$(SRCS:.c=.o))
LDFLAGS=-L../../lib -lmenu -lhash -lpthread -lserver -larray -ldl
TARGET=server_main
INC=-I../../include
CFLAGS= -g -fPIC $(INC)

all:$(TARGET) libadd.so libsub.so liblogin.so libjoin.so \
	libpageInfo.so libaddNewPost.so libdeletePost.so \
	libboardSel.so libdownloadSel.so libchattingSel.so \
	libmenu2Sel.so libarchieveInfo.so libarchieveNew.so libarchieveDelete.so \
	libchatting.so libsearchPost.so libarchieveSearch.so libmenu1Sel.so

$(TARGET):$(OBJS)
	gcc -o $@ $^ $(LDFLAGS)
	cp $@ ../../bin
	
libadd.so: add.o 
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/

libsub.so: sub.o 
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
liblogin.so: login.o 
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
libjoin.so: join.o 
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
libpageInfo.so: pageInfo.o
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
libaddNewPost.so: addNewPost.o
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
libdeletePost.so: deletePost.o
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
libboardSel.so: boardSel.o
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
libdownloadSel.so: downloadSel.o
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
libchattingSel.so: chattingSel.o
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
libmenu1Sel.so: menu1Sel.o
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
libmenu2Sel.so: menu2Sel.o
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
libarchieveInfo.so: archieveInfo.o
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
libarchieveNew.so: archieveNew.o
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
libarchieveSearch.so: archieveSearch.o
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
libarchieveDelete.so: archieveDelete.o
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
libchatting.so: chatting.o
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
libsearchPost.so: searchPost.o
	$(LD) -shared -o $@ $<
	cp $@ ../../lib/
clean:
	rm $(OBJS) $(TARGET) *.so  
	rm *.o
