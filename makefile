CC = g++
FLAGS = -Wall -std=c++17

run: menu
	./menu
	
menu: main.o menu.o
	$(CC) $(FLAGS) $^ -o menu -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp GUI-Components/menu.h
	$(CC) $(FLAGS) -c $< -o main.o

menu.o: GUI-Components/menu.cpp GUI-Components/menu.h
	$(CC) $(FLAGS) -c $< -o menu.o

game: main.o button.o InputHandler.o
	$(CC) $(FLAGS) main.o button.o InputHandler.o -o game -lsfml-graphics -lsfml-window -lsfml-system
	mv *.o ObjectFiles

main.o: main.cpp Handlers/InputHandler/InputHandler.h
	$(CC) $(FLAGS) -c main.cpp -o main.o

InputHandler.o: Handlers/InputHandler/InputHandler.cpp Handlers/InputHandler/InputHandler.h
	$(CC) $(FLAGS) -c Handlers/InputHandler/InputHandler.cpp -o InputHandler.o

clean:
	rm -f *.o game menu main
