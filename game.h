/**
 * @file game.h
 * @author Konner Knoll
 * @brief Game class
 * @version 0.2
 * @date 2026-04-28
 */

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Handlers/WaveHandler/WaveHandler.h"
#include "Entities/Entities.h"

class Game
{
public:
    Game();
    ~Game() = default;

    // Game class should not be copyable
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void run();

private:
    enum class State { Menu, Playing, Paused };
    enum class Difficulty { None, Easy, Medium, Hard };

    void processEvents();
    void update(float deltaTime);
    void render();

    sf::RenderWindow mWindow;
    sf::Clock mClock;

    State mState { State::Playing };
    Difficulty mDifficulty { Difficulty::None };

    Tower mTower;
    WaveHandler mWaves;

    sf::Music mMusic;
};

#endif