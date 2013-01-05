#the makefile for shortestPath

all : shortestPath

shortestPath : shortestPath.o function.o
	gcc shortestPath.o function.o -o shortestPath

shortestPath.o : shortestPath.c function.h
	gcc -c shortestPath.c

function.o : function.c function.h
	gcc -c function.c

#cleaning executive file and object file
.PHONY : clean

clean :
	rm -rf shortestPath *.o
