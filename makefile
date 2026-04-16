CC = g++
FLAGS = -Wall -std=c++17

run: tower
	./tower

tower: entities.o hurtbox.o main.o
	$(CC) $(FLAGS) $^ -o tower -lsfml-graphics -lsfml-window -lsfml-system

entities.o: Entities/Entities.cpp Entities/Entities.h
	$(CC) $(FLAGS) -c $< -o entities.o

hurtbox.o: DetectionTools/Hurtbox/Hurtbox.cpp DetectionTools/Hurtbox/Hurtbox.h
	$(CC) $(FLAGS) -c $< -o hurtbox.o

hitbox.o: DetectionTools/Hitbox/Hitbox.cpp DetectionTools/Hitbox/Hitbox.h
	$(CC) $(FLAGS) -c $< -o hitbox.o

main.o: main.cpp Entities/Entities.h Handlers/InputHandler/InputHandler.h
	$(CC) $(FLAGS) -c $< -o main.o

menu: main.o menu.o
	$(CC) $(FLAGS) $^ -o menu -lsfml-graphics -lsfml-window -lsfml-system

menu.o: GUI-Components/menu.cpp GUI-Components/menu.h
	$(CC) $(FLAGS) -c $< -o menu.o

game: main.o button.o InputHandler.o
	$(CC) $(FLAGS) $^ -o game -lsfml-graphics -lsfml-window -lsfml-system
	mv *.o ObjectFiles

InputHandler.o: Handlers/InputHandler/InputHandler.cpp Handlers/InputHandler/InputHandler.h
	$(CC) $(FLAGS) -c $< -o InputHandler.o

EntityHandler.o: Handlers/EntityHandler/EntityHandler.cpp Handlers/EntityHandler/EntityHandler.h
	$(CC) $(FLAGS) -c $< -o EntityHandler.o

clean:
	rm -f *.o tower game menu