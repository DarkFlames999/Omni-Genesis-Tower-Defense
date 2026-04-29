CC = g++
FLAGS = -Wall -std=c++17
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

run: game
	./game

game: entities.o hurtbox.o hitbox.o main.o InputHandler.o EntityHandler.o WaveHandler.o game.o
	$(CC) $(FLAGS) $^ -o game $(LIBS)

entities.o: Entities/Entities.cpp Entities/Entities.h
	$(CC) $(FLAGS) -c $< -o $@

hurtbox.o: DetectionTools/Hurtbox/Hurtbox.cpp DetectionTools/Hurtbox/Hurtbox.h
	$(CC) $(FLAGS) -c $< -o $@

hitbox.o: DetectionTools/Hitbox/Hitbox.cpp DetectionTools/Hitbox/Hitbox.h
	$(CC) $(FLAGS) -c $< -o $@

main.o: main.cpp game.h
	$(CC) $(FLAGS) -c $< -o $@

game.o: game.cpp game.h Entities/Entities.h Handlers/WaveHandler/WaveHandler.h
	$(CC) $(FLAGS) -c $< -o $@

InputHandler.o: Handlers/InputHandler/InputHandler.cpp Handlers/InputHandler/InputHandler.h
	$(CC) $(FLAGS) -c $< -o $@

EntityHandler.o: Handlers/EntityHandler/EntityHandler.cpp Handlers/EntityHandler/EntityHandler.h
	$(CC) $(FLAGS) -c $< -o $@

WaveHandler.o: Handlers/WaveHandler/WaveHandler.cpp Handlers/WaveHandler/WaveHandler.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f *.o game