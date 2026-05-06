CC = g++
FLAGS = -Wall -std=c++17
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

run: game
	./game

tests: TESTS/test
	./TESTS/test

game: entities.o hurtbox.o hitbox.o main.o InputHandler.o EntityHandler.o WaveHandler.o CollisionHandler.o SkillTree.o game.o menu.o MagicSelection.o SkillTreeView.o
	$(CC) $(FLAGS) $^ -o game $(LIBS)

menu.o: GUI-Components/menu.cpp GUI-Components/menu.h
	$(CC) $(FLAGS) -c $< -o $@

MagicSelection.o: GUI-Components/MagicSelection.cpp GUI-Components/MagicSelection.h
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

SkillTreeView.o: SkillTree/SkillTreeView.cpp SkillTree/SkillTreeView.h
	$(CC) $(FLAGS) -c $< -o $@

TESTS/test.o: TESTS/test.cpp
	$(CC) $(FLAGS) -c $< -o $@

TESTS/catch_amalgamated.o: TESTS/catch_amalgamated.cpp
	$(CC) $(FLAGS) -c $< -o $@

TESTS/test: TESTS/test.o TESTS/catch_amalgamated.o entities.o hurtbox.o hitbox.o InputHandler.o EntityHandler.o WaveHandler.o CollisionHandler.o SkillTree.o SkillTreeView.o MagicSelection.o menu.o
	$(CC) $(FLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f *.o game