/**
 * @file main.cpp
 * @author Keali Lake
 * @brief  This program testing class buton
 * @version 0.1
 * @date 2022-10-16
 * 
 * @copyright Copyright (c) 2022
 * @aknolegment https://code.markrichards.ninja/sfml/how-to-create-simple-buttons-for-your-sfml-game
 * 
 * 
 */
#include <SFML/Graphics.hpp>
#include "Handlers/InputHandler/InputHandler.h"
#include <iostream>
#include "GUI-Components/menu.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 500), "Omni-Genesis/TowerDefense");
    window.setFramerateLimit(60);

    starAnimation(window);

    Title title("Omni-Genesis/TowerDefense");
    sf::Clock titleClock;
    bool titleDone = false;

    //Play buttons appear after the title is done appearing
    Button play("Play",{180,150}, {230, 100}, sf::Color(112, 3, 0, 255));
    Button custom("Custom", {180,280}, {230, 100}, sf::Color(112, 3, 0, 255));
    Button credit("Credits", {180, 410}, {230, 100}, sf::Color(112, 3, 0, 255));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(titleDone)
            {
                play.update(event, window);
                custom.update(event, window);
                credit.update(event, window);
            }
        }
        window.clear(sf::Color::Black);

        if(titleClock.getElapsedTime().asSeconds() >= 1.0f)
        {
            title.update(window);
            title.draw(window);
            if(title.titleX <= 101.0f && title.titleY <= 41.0f)
            {
                titleDone = true;
            }
        }

        if(titleDone)
        {
            play.setColorButtonFade();
            custom.setColorButtonFade();
            credit.setColorButtonFade();

            title.draw(window);
            window.draw(play);
            window.draw(custom);
            window.draw(credit);
        }
        window.display();
    }

    return 0;
}

int main()
{
   sf::RenderWindow window(sf::VideoMode({800, 500}), "Omni-Genesis-Tower-Defense", sf::Style::Titlebar | sf::Style::Close);
   InputHandler inputHandler;
   inputHandler.ImplementHeldKey(sf::Keyboard::W, 1);
   inputHandler.ImplementDoubleTap(sf::Keyboard::Num1, 0.5);

    sf::RectangleShape rectangle(sf::Vector2f(100.f, 100.f));
    window.setFramerateLimit(60);

   while (window.isOpen())
   {
       sf::Event event;
       
       inputHandler.update(event);

       while (window.pollEvent(event))
       {
           if (event.type == sf::Event::Closed){
               window.close();
            };

            if(inputHandler.isHeldKey(inputHandler.mHeldKeyBindings[0], event) == true)
            {
                
                rectangle.setFillColor(sf::Color::Red);
                rectangle.setPosition(350.f, 200.f);
            }

            if(inputHandler.isDoubleTap(inputHandler.mDoubleTapBindings[0], event) == true)
            {
                rectangle.setFillColor(sf::Color::Blue);
                rectangle.setPosition(200.f, 400.f);
            }

       }

       window.clear();

       window.draw(rectangle);

       window.display();
   }

   return 0;
}