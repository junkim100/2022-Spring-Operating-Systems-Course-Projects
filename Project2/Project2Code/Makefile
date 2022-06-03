CC = g++ -std=c++11
CFLAGS = -c
# CFLAGS = -c -Wall

memsim: main.o
	$(CC) main.o -o memsim
	rm *.o

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clean: 
	rm memsim