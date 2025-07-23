# Typing 'make' or 'make all' will create the executable file.
#

all:main

main: serverM.o serverA.o serverR.o serverD.o client.o
      gcc -g -Wall -o main serverM.o serverA.o serverR.o serverD.o client.o

serverM.o: serverM.c serverM.h
	   gcc -g -Wall -c serverM.c

serverR.o: serverR.c serverR.h
	   gcc -g -Wall -c serverR.c

serverD.o: serverD.c serverD.h
	   gcc -g -Wall -c serverD.c

client.o: client.c client.h
	   gcc -g -Wall -c client.c



clean:
	rm -f *.o main



