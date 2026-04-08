CC = g++
FLAGS = -Wall -std=c++17

run: menu
	./menu
	
menu: main.o menu.o
	$(CC) $(FLAGS) $^ -o menu -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp menu.h
	$(CC) $(FLAGS) -c $< -o main.o

menu.o: menu.cpp menu.h
	$(CC) $(FLAGS) -c $< -o menu.o

clean: 
	rm -f *.o menu