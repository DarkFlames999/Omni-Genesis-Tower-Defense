CC = g++
FLAGS = -Wall -std=c++17 

run: game
	./game

game: entities.o main.o EntityHandler.o InputHandler.o  WaveHandler.o CollisionHandler.o hitbox.o hurtbox.o
	$(CC) $(FLAGS) $^ -o game -lsfml-graphics -lsfml-window -lsfml-system

entities.o: Entities/Entities.cpp Entities/Entities.h
	$(CC) $(FLAGS) -c $< -o entities.o

hitbox.o: DetectionTools/Hitbox.cpp DetectionTools/Hitbox.h
	$(CC) $(FLAGS) -c $< -o hitbox.o

hurtbox.o: DetectionTools/Hurtbox.cpp DetectionTools/Hurtbox.h
	$(CC) $(FLAGS) -c $< -o hurtbox.o

main.o: main.cpp Entities/Entities.h Handlers/InputHandler/InputHandler.h Handlers/EntityHandler/EntityHandler.h
	$(CC) $(FLAGS) -c $< -o main.o

InputHandler.o: Handlers/InputHandler/InputHandler.cpp Handlers/InputHandler/InputHandler.h
	$(CC) $(FLAGS) -c $< -o InputHandler.o

EntityHandler.o: Handlers/EntityHandler/EntityHandler.cpp Handlers/EntityHandler/EntityHandler.h
	$(CC) $(FLAGS) -c $< -o EntityHandler.o

CollisionHandler.o: Handlers/CollisionHandler/CollisionHandler.cpp Handlers/CollisionHandler/CollisionHandler.h
	$(CC) $(FLAGS) -c $< -o CollisionHandler.o

WaveHandler.o: Handlers/WaveHandler/WaveHandler.cpp Handlers/WaveHandler/WaveHandler.h
	$(CC) $(FLAGS) -c $< -o WaveHandler.o

clean:
	rm -f *.o tower game menu
