# Typing 'make all' will create the executable file.
#

all:serverM serverA serverR serverD client


serverM: serverM.o

serverM.o: serverM.c serverM.h
	gcc -g -Wall -c serverM.c

serverA:serverA.o

serverA.o: serverA.c serverA.h
	gcc -g -Wall -c serverA.c

serverR:serverR.o

serverR.o: serverR.c serverR.h
	gcc -g -Wall -c serverR.c

serverD:serverD.o

serverD.o: serverD.c serverD.h
	gcc -g -Wall -c serverD.c


client:client.o

client.o:client.c client.h
	gcc -g -Wall -c client.c



clean:
	rm -f *.o main



