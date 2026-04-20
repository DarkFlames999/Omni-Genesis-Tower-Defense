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

int main()
{
    sf::RenderWindow window(sf::VideoMode(800,500), "Omni-Genesis/TowerDefense");
    window.setFramerateLimit(60);

    Juvenile Juvenile;
    Juvenile.createJuvenile(window, {600.0f, 400.0f}, {40.0f, 130.0f}, {30.f, 120.f});

    Tower tower;
    tower.createTower({400.0f, 400.0f}, {40.0f, 130.0f}, {10.f, 10.f});

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close(); 
            }
        }
        tower.update(window);
        Juvenile.update(window);
        window.clear(sf::Color::Black);
        window.draw(tower);
        window.draw(Juvenile);
        window.display();
    }

//TESTING THE MAIN MENU
    // starAnimation(window);

    // Title title("Omni-Genesis/TowerDefense");
    // sf::Clock titleClock;
    // bool titleDone = false;

    // //Play buttons appear after the title is done appearing
    // Button play("Play",{180,150}, {230, 100}, sf::Color(112, 3, 0, 255));
    // Button custom("Custom", {180,280}, {230, 100}, sf::Color(112, 3, 0, 255));
    // Button credit("Credits", {180, 410}, {230, 100}, sf::Color(112, 3, 0, 255));

    // while (window.isOpen())
    // {
    //     sf::Event event;
    //     while (window.pollEvent(event))
    //     {
    //         if (event.type == sf::Event::Closed)
    //             window.close();
    //         if(titleDone)
    //         {
    //             play.update(event, window);
    //             custom.update(event, window);
    //             credit.update(event, window);
    //         }
    //     }
    //     window.clear(sf::Color::Black);

    //     if(titleClock.getElapsedTime().asSeconds() >= 1.0f)
    //     {
    //         title.update(window);
    //         title.draw(window);
    //         if(title.titleX <= 101.0f && title.titleY <= 41.0f)
    //         {
    //             titleDone = true;
    //         }
    //     }

    //     if(titleDone)
    //     {
    //         play.setColorButtonFade();
    //         custom.setColorButtonFade();
    //         credit.setColorButtonFade();

    //         title.draw(window);
    //         window.draw(play);
    //         window.draw(custom);
    //         window.draw(credit);
    //     }
    //     window.display();
    // }

    return 0;
}
