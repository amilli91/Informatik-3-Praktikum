CC=g++

//CFLAGS=$(shell pkg-config --cflags opencv) 
//LIBS=$(shell pkg-config --libs opencv) 

OBJS= main.o  TASK1.o TASK2.o TASK4.o SHA256.o SIMPLESOCKET.o
PWDOBJS=PwdServerApp.o PwdServer.o TASK1.o SHA256.o SIMPLESOCKET.o
ALLTARGET=main server client mainTest PwdClient PwdServerApp
PWDTARGET=PwdClient PwdServerApp

client.o:	client.cpp
	$(CC) -c $<  -std=c++11

server.o:	server.cpp
	$(CC) -c $<  -std=c++11

PwdServer.o:	PwdServer.cpp
	$(CC) -c $<  -std=c++11

PwdClient.o:	PwdClient.cpp
	$(CC) -c $<  -std=c++11

PwdServerApp.o:	PwdServerApp.cpp
	$(CC) -c $<  -std=c++11

SIMPLESOCKET.o:	SIMPLESOCKET.cpp
	$(CC) -c $<  -std=c++11

SHA256.o:	SHA256.cpp
	$(CC) -c $<  -std=c++11

TASK1.o:	TASK1.cpp
	$(CC) -c $<  -std=c++11

TASK2.o:	TASK2.cpp
	$(CC) -c $<  -std=c++11

TASK4.o:	TASK4.cpp
	$(CC) -c $<  -std=c++11 

main.o:	main.cpp
	$(CC) -c $<  -std=c++11	

mainTest.o:	mainTest.cpp
	$(CC) -c $<  -std=c++11		



main:	$(OBJS)
	$(CC) -o $@ $^ -L/usr/lib/x86_64-linux-gnu -ldl -lstdc++  -std=c++11 -lpthread $(LIBS)

mainTest:	mainTest.o
	$(CC) -o $@ $^ TASK1.o SHA256.o -L/usr/lib/x86_64-linux-gnu -ldl -lstdc++  -std=c++11 -lpthread $(LIBS)

PwdClient: PwdClient.o SIMPLESOCKET.o
	$(CC) -o $@ $^ -L/usr/lib/x86_64-linux-gnu -ldl -lstdc++  -std=c++11 

PwdServerApp: $(PWDOBJS)
	$(CC) -o $@ $^ -L/usr/lib/x86_64-linux-gnu -ldl -lstdc++  -std=c++11

server:	server.o
	$(CC) -o server server.o  SIMPLESOCKET.o -L/usr/lib/x86_64-linux-gnu -ldl -lstdc++  -std=c++11

client:	client.o
	$(CC) -o client client.o SIMPLESOCKET.o -L/usr/lib/x86_64-linux-gnu -ldl -lstdc++  -std=c++11

Pwd:	$(PWDTARGET)
	make clean && make PwdClient && make PwdServerApp

clean:
	-rm -r -f   $(ALLTARGET) *.o DOXYGENDOC  *.txt

doc:
	doxygen Doxyfile 

all:	$(ALLTARGET)
	make clean  && make main && make server && make client && make mainTest && make PwdClient && make PwdServerApp

run:	main	
	./main

