lab08-c:
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -c lab08.cpp

lab08-l: lab08-c
	g++ -Wall -std=c++14 -g -O0 -pedantic-errors -o cat lab08.o

clean:
	rm -f *.o
	rm -f cat
	rm -f *~
	rm -f text*.txt

backup:
	cp lab08.cpp lab08_back.cpp

restore:
	cp lab08_back.cpp lab08.cpp