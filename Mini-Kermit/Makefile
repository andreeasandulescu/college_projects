all: build 

build: ksender kreceiver

ksender: ksender.o link_emulator/lib.o kfunc.o
	gcc -g ksender.o link_emulator/lib.o kfunc.o -o ksender

kreceiver: kreceiver.o link_emulator/lib.o kfunc.o
	gcc -g kreceiver.o link_emulator/lib.o kfunc.o -o kreceiver

kfunc.o: kfunc.c
	gcc -g -c kfunc.c

.c.o: 
	gcc -Wall -g -c $? 

clean:
	-rm -f *.o ksender kreceiver 
