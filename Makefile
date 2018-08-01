SOURCES = main.cpp
CC = g++
FLAG = -Wall -std=c++11 -pthread -o

All: 
	$(CC) $(FLAG) multi $(SOURCES) 

clean:
	rm multi 
