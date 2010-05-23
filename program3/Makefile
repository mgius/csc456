HANDINACCT = pn-cs456
HANDINDIR = Asgn3
HANDINFILES = "*.cpp Makefile *.h README key.* mgius.*"
CXXFLAGS = -Wall -ansi -g

all: mksuid

run: mksuid rootify

mksuid: mksuid.o
	g++ -lcrypt -o $@ $^

rootify: mksuid
	sudo chown root:mgius mksuid
	sudo chmod 4775 mksuid

clean: 
	rm -f *.o mksuid a.out

handin: all
	handin $(HANDINACCT) $(HANDINDIR) $(HANDINFILES)
	make handincheck
	make clean

handincheck:
	handin $(HANDINACCT) $(HANDINDIR)
