HANDINACCT = pn-cs456
HANDINDIR = Asgn1
HANDINFILES = "ftable.cpp ftable.pl Makefile tests.tar vig.cpp vig.pl README"
CXXFLAGS = -Wall -ansi

all: ftable vig

ftable: ftable.cpp
	g++ $(CXXFLAGS) ftable.cpp -lm -o ftable

vig: vig.cpp
	g++ $(CXXFLAGS) vig.cpp -o vig

test: all testtar
	tar -xf tests.tar
	sh runAllTests.sh

clean:
	rm -f vig ftable tests.tar

testtar:
	tar -cf tests.tar runAllTests.sh testRunner.pl testcases/

handin: all testtar test
	handin $(HANDINACCT) $(HANDINDIR) $(HANDINFILES)
	make handincheck
	make clean

handincheck:
	handin $(HANDINACCT) $(HANDINDIR)
	
