CC = g++
FLAGS = -Wall -std=c++17
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

run: game
	./game

game: entities.o hurtbox.o hitbox.o main.o InputHandler.o EntityHandler.o WaveHandler.o CollisionHandler.o SkillTree.o game.o menu.o
	$(CC) $(FLAGS) $^ -o game $(LIBS)

menu.o: GUI-Components/menu.cpp GUI-Components/menu.h
	$(CC) $(FLAGS) -c $< -o $@

entities.o: Entities/Entities.cpp Entities/Entities.h
	$(CC) $(FLAGS) -c $< -o $@

hurtbox.o: DetectionTools/Hurtbox.cpp DetectionTools/Hurtbox.h
	$(CC) $(FLAGS) -c $< -o $@

hitbox.o: DetectionTools/Hitbox.cpp DetectionTools/Hitbox.h
	$(CC) $(FLAGS) -c $< -o $@

main.o: main.cpp game.h
	$(CC) $(FLAGS) -c $< -o $@

game.o: game.cpp game.h Entities/Entities.h Handlers/WaveHandler/WaveHandler.h
	$(CC) $(FLAGS) -c $< -o $@

InputHandler.o: Handlers/InputHandler/InputHandler.cpp Handlers/InputHandler/InputHandler.h
	$(CC) $(FLAGS) -c $< -o $@

EntityHandler.o: Handlers/EntityHandler/EntityHandler.cpp Handlers/EntityHandler/EntityHandler.h
	$(CC) $(FLAGS) -c $< -o $@

CollisionHandler.o: Handlers/CollisionHandler/CollisionHandler.cpp Handlers/CollisionHandler/CollisionHandler.h
	$(CC) $(FLAGS) -c $< -o CollisionHandler.o

WaveHandler.o: Handlers/WaveHandler/WaveHandler.cpp Handlers/WaveHandler/WaveHandler.h
	$(CC) $(FLAGS) -c $< -o $@

SkillTree.o: SkillTree/SkillTree.cpp SkillTree/SkillTree.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f *.o game