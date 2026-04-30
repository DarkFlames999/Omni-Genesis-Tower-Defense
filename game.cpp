/**
 * @file game.cpp
 * @author Konner Knoll
 * @brief Game class definitions
 * @version 0.2
 * @date 2026-04-28
 */
#include "game.h"
#include <iostream>

/**
 * @brief Construct a new Game:: Game object
 * 
 */
Game::Game()
    : mWindow(sf::VideoMode::getDesktopMode(),
              "Omni-Genesis/TowerDefense",
              sf::Style::Fullscreen)
{
    mWindow.setFramerateLimit(60);
    mTower.createTower(mWindow, {40.0f, 130.0f}, {10.f, 10.f});
    mWaves.StartNextWave(mWindow);

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
    // Background music
    if (!mMusic.openFromFile("music/background.ogg"))
        std::cerr << "Warning: failed to load background music\n";
    mMusic.setLoop(true);
    mMusic.setVolume(50.f);
    mMusic.play();
}

/**
 * @brief Run the game
 * 
 */
void Game::run()
{
    while (mWindow.isOpen())
    {
        float deltaTime = mClock.restart().asSeconds();
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        processEvents();
        update(deltaTime);
        render();
    }
}

/**
 * @brief Activates the input handler
 * 
 */
void Game::activateInputHandler(){
    
 * @brief Process game events
 * 
 */
void Game::processEvents()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            mWindow.close();

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape)
            mWindow.close();

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
            mTower.shoot(mWindow);
    }
}

/**
 * @brief Activates the wave handler
 * 
 */
void Game::activateWaveHandler(){

}

// add more functions for each new handler
 * @brief Update game
 * 
 * @param deltaTime 
 */
void Game::update(float deltaTime)
{
    mTower.update(mWindow, deltaTime);
    mTower.updateAttack(mWindow, deltaTime);
    mWaves.Update(mWindow, deltaTime);

    if (mWaves.IsWaveComplete())
        mWaves.StartNextWave(mWindow);
}

/**
 * @brief Render game
 * 
 */
void Game::render()
{
    mWindow.clear(sf::Color::Black);
    mWindow.draw(mTower);
    mTower.drawAttack(mWindow);
    mWaves.DrawEntities(mWindow, sf::RenderStates::Default);
    mWindow.display();
}
