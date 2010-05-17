HANDINACCT = pn-cs456
HANDINDIR = Asgn2
HANDINFILES = "ftable.cpp Makefile vig.cpp README"
CXXFLAGS = -Wall -ansi

all: ftable vig

ftable: ftable.cpp
	g++ $(CXXFLAGS) ftable.cpp -lm -o ftable

vig: vig.cpp
	g++ $(CXXFLAGS) vig.cpp -o vig

ic: ic.o
	g++ $(CXXFLAGS) ic.cpp -o ic

clean:
	rm -f vig ftable ic *.o

handin: all
	handin $(HANDINACCT) $(HANDINDIR) $(HANDINFILES)
