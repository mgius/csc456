HANDINACCT = pn-cs456
HANDINDIR = Asgn2
HANDINFILES = "ftable.cpp Makefile vig.cpp README"
CXXFLAGS = -Wall -ansi -g

all: ftable vig ic

ftable: ftable.o ic.o
	g++ $(CXXFLAGS) $^ -lm -o ftable

vig: vig.cpp
	g++ $(CXXFLAGS) vig.cpp -o vig

ic: icMain.o ic.o
	g++ $(CXXFLAGS) $^ -o ic

kasiski: kasiski.o
	g++ $(CXXFLAGS) $^ -o kasiski

clean:
	rm -f vig ftable ic kasiski *.o

handin: all
	handin $(HANDINACCT) $(HANDINDIR) $(HANDINFILES)
