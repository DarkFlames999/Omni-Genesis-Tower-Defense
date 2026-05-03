/**
 * @file main.cpp
 * @author Isaiah Watkins, Konner Knoll, and Keali Laker
 * @brief This is the main file for the Omni-Genesis/TowerDefense game, which will be used to create the window, run the main game loop, and call all the relevant functions to update and draw the game.
 * @version 0.1
 * @date 2026-4-15
 * @copyright Copyright (c) 2022
 */
/**
 * @file main.cpp
 * @author Isaiah Watkins, Konner Knoll, and Keali Laker
 * @brief This is the main file for the Omni-Genesis/TowerDefense game, which will be used to create the window, run the main game loop, and call all the relevant functions to update and draw the game.
 * @version 0.1
 * @date 2026-4-15
 * @copyright Copyright (c) 2022
 */
#include <SFML/Graphics.hpp>
#include "Handlers/InputHandler/InputHandler.h"
#include <iostream>
#include "GUI-Components/menu.h"
#include "Entities/Entities.h"
#include "Handlers/EntityHandler/EntityHandler.h"
#include "Handlers/WaveHandler/WaveHandler.h"
#include "Handlers/CollisionHandler/CollisionHandler.h"
#include "SkillTree/SkillTree.h"
#include "game.h"

int main()
{
    Game game;
    game.run();
    return 0;
}