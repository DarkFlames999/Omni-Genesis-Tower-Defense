/**
 * @file game.cpp
 * @author Konner Knoll
 * @brief Game class definitions
 * @version 0.1
 * @date 2026-04-08
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "game.h"
#include "Handlers/EntityHandler/EntityHandler.h"
#include "Handlers/InputHandler/InputHandler.h"
#include "Handlers/WaveHandler/WaveHandler.h"
#include "Handlers/CollisionHandler/CollisionHandler.h"

/**
 * @brief Plays the game
 * 
 */
void Game::playGame(){
    activateHandlers();
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Omni-Genesis/TowerDefense", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close(); 
            }
            if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    tower.shoot(window);
                }
            }
            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();
        if(deltaTime > 0.1f) deltaTime = 0.1f;

        tower.update(window, deltaTime);
        tower.updateAttack(window, deltaTime);
        // entityhandler.UpdateEntities(window);
        // Juvenile.update(window);
        // matured.update(window);
        // warden.update(window);

        waves.Update(window, deltaTime);

        window.clear(sf::Color::Black);
        window.draw(background);
        window.draw(tower);
        tower.drawAttack(window);
        waves.DrawEntities(window, sf::RenderStates::Default);
        window.display();
    }
}

/**
 * @brief Activates all the handlers when relevant
 * 
 */
void Game::activateHandlers(){
    // put code here to call handlers
    activateInputHandler();
    activateEntityHandler();
}

/**
 * @brief Activates the input handler
 * 
 */
void Game::activateInputHandler(){
    

}

/**
 * @brief Activates the entity handler
 * 
 */
void Game::activateEntityHandler(){
    
}

/**
 * @brief Activates the wave handler
 * 
 */
void Game::activateWaveHandler(){

}

// add more functions for each new handler