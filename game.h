/**
 * @file game.h
 * @author Konner Knoll
 * @brief Define game state class, which manages all the handlers
 * @version 0.1
 * @date 2026-04-08
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef GAME_H
#define GAME_H

class Game{
public:
    Game(){};
    ~Game(){};
    void playGame();
    void activateHandlers();
    void activateInputHandler();
    void activateEntityHandler();
    void activateWaveHandler();
private:
    enum mState{menu, game, paused};
    enum mDifficulty{none, easy, medium, hard};
};

#endif