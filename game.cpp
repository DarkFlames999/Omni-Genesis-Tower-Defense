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
 * @brief Update game
 * 
 * @param deltaTime 
 */
void Game::update(float deltaTime)
{
    mTower.update(mWindow, deltaTime);
    mTower.updateAttack(mWindow, deltaTime);
    mWaves.Update(mWindow, deltaTime);
    mCollisionHandler.checkBulletEnemyCollision(mTower.getAttacks(), mWaves);
    mCollisionHandler.checkEnemyTowerCollision(mWaves, mTower);

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