CXXFLAGS = -Wall
all: ftable vig

ftable: ftable.cpp
	g++ $(CXXFLAGS) ftable.cpp -lm -o ftable

vig: vig.cpp
	g++ $(CXXFLAGS) vig.cpp -o vig

clean:
	rm -f vig ftable
