CC = g++
FLAGS = -Wall -std=c++17

game: main.o button.o InputHandler.o
	$(CC) $(FLAGS) main.o button.o InputHandler.o -o game -lsfml-graphics -lsfml-window -lsfml-system
	mv *.o ObjectFiles

main.o: main.cpp GUI-Components/button.h Handlers/InputHandler/InputHandler.h
	$(CC) $(FLAGS) -c main.cpp -o main.o

button.o: GUI-Components/button.cpp GUI-Components/button.h
	$(CC) $(FLAGS) -c GUI-Components/button.cpp -o button.o

InputHandler.o: Handlers/InputHandler/InputHandler.cpp Handlers/InputHandler/InputHandler.h
	$(CC) $(FLAGS) -c Handlers/InputHandler/InputHandler.cpp -o InputHandler.o

clean:
	rm -f *.o game