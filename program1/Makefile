HANDINACCT = pn-cs456
HANDINDIR = Asgn1
HANDINFILES = "ftable.cpp ftable.pl Makefile tests.tar vig.cpp vig.pl"
CXXFLAGS = -Wall
all: ftable vig

ftable: ftable.cpp
	g++ $(CXXFLAGS) ftable.cpp -lm -o ftable

vig: vig.cpp
	g++ $(CXXFLAGS) vig.cpp -o vig

test: all
	tar -xf tests.tar
	sh runAllTests.sh

clean:
	rm -f vig ftable tests.tar

testtar:
	tar -cf tests.tar runAllTests.sh testRunner.pl testcases/

handin: all testtar test
	handin $(HANDINACCT) $(HANDINDIR) $(HANDINFILES)
