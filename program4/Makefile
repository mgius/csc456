HANDINACCT = pn-cs456
HANDINDIR = Asgn4
HANDINFILES = "*.c Makefile README smashme"
CFLAGS = --std=gnu99 -g

all: smashme configure

configure: configure.o

smashme: smashme.o
	$(CC) -o $@ $^

clean: 
	rm -f *.o smashme a.out *.s

handin: all
	handin $(HANDINACCT) $(HANDINDIR) $(HANDINFILES)
	@make --no-print-directory handincheck
	@make --no-print-directory clean

handincheck:
	handin $(HANDINACCT) $(HANDINDIR)
