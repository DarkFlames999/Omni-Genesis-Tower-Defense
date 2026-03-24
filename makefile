CC = g++
FLAGS = -Wall -std=c++17

button: main.o button.o
	$(CC) $(FLAGS) $^ -o button -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp button.h
	$(CC) $(FLAGS) -c $< -o main.o

button.o: button.cpp button.h
	$(CC) $(FLAGS) -c $< -o button.o