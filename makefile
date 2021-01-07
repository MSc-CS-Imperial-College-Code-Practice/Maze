exec: main.o maze.o
	  	g++ -Wall -g main.o maze.o -o exec

main.o: main.cpp maze.h
		g++ -Wall -g -c main.cpp

maze.o: maze.cpp maze.h
		  g++ -Wall -g -c maze.cpp 

clean:
	  rm -f *.o exec