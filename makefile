CC = g++
FLAGS = -Wall -std=c++17
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

run: game
	./game

game: entities.o hurtbox.o hitbox.o main.o InputHandler.o EntityHandler.o WaveHandler.o CollisionHandler.o SkillTree.o game.o menu.o MagicSelection.o SkillTreeView.o
	$(CC) $(FLAGS) $^ -o game $(LIBS)

#Pattern rule
%.o: %.cpp
$(CC) $(FLAGS) -c $< -o $@

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

TESTS/test: TESTS/test.o TESTS/catch_amalgamated.o SkillTreeView.o Skill.o WaveHandler.o CollisionHandler.o EntityHandler.o InputHandler.o game.o main.o hitbox.o hurtbox.o entities.o magic selection.o menu.o
	$(CC) $(FLAGS)  $^ -o $@

TESTS/test.o:  TESTS/catch_amalgamated.hpp SkillTree/SkillTreeView.h Handlers/WaveHandler/WaveHandler.h Handlers/CollisionHandler/CollisionHandler.h Handlers/EntityHandler/EntityHandler.h Handlers/InputHandler/InputHandler.h Handlers/WaveHandler/WaveHandler.h DetectionTools/Hitbox.h DetectionTools/Hurtbox.h Entities/Entities.h GUI-Components/MagicSelection.h GUI-Components/menu.h
	$(CC) $(FLAGS)  $^ -o $@

TESTS/catch_amalgamated.o:  TESTS/catch_amalgamated.hpp 
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f *.o game