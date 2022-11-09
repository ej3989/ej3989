.PHONY:libsrc server client ref_server clean

all:libsrc server client #ref_server 

libsrc:
	$(MAKE) -C libsrc

server:
	$(MAKE) -C server

client:
	$(MAKE) -C client

ref_server:
	$(MAKE) -C ref_server

clean:
	cd libsrc; $(MAKE) clean 
	cd server; $(MAKE) clean 
	cd client; $(MAKE) clean 
#	cd ref_server; $(MAKE) clean 
