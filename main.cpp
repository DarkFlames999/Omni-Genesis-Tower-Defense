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

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Omni-Genesis/TowerDefense", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // Warden warden;
    // warden.createWarden(window, {600.0f, 400.0f}, {40.0f, 130.0f}, {30.f, 120.f});

    // Matured matured;
    // matured.createMatured(window, {600.0f, 400.0f}, {40.0f, 130.0f}, {30.f, 120.f});

    // Juvenile Juvenile;
    // Juvenile.createJuvenile(window, {600.0f, 400.0f}, {40.0f, 130.0f}, {30.f, 120.f});

    Tower tower;
    tower.createTower(window,{40.0f, 130.0f}, {10.f, 10.f});

    WaveHandler waves;
    
    waves.StartNextWave(window);

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
        window.draw(tower);
        tower.drawAttack(window);
        waves.DrawEntities(window, sf::RenderStates::Default);
        // window.draw(Juvenile);
        // window.draw(matured);
        // window.draw(warden);
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
