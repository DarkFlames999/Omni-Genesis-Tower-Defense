/**
 * @file game.h
 * @author Konner Knoll
 * @brief Game class
 * @version 0.1
 */
#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Handlers/WaveHandler/WaveHandler.h"
#include "GUI-Components/menu.h"
#include "Entities/Entities.h"
#include "Handlers/EntityHandler/EntityHandler.h"
#include "Handlers/CollisionHandler/CollisionHandler.h"
#include "Handlers/InputHandler/InputHandler.h"
#include "SkillTree/SkillTree.h"
#include <memory>

class Game
{
public:
    Game();
    ~Game() = default;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    void run();

private:
    enum class State       { Intro, Menu, DifficultySelect, Playing, Paused };
    enum class Difficulty  { None, Easy, Medium, Hard };

    void processEvents();
    void update(float dt);
    void render();

    void updateIntro();
    void renderIntro();

    void initMenu();
    void updateMenu(sf::Event& e);
    void renderMenu();

    void initDifficultySelect();
    void updateDifficultySelect(sf::Event& e);
    void renderDifficultySelect();

    void startGame();
    void updatePlaying(float dt);
    void renderPlaying();

    sf::RenderWindow mWindow;
    sf::Clock mClock;
    State mState { State::Intro };
    Difficulty mDifficulty { Difficulty::None };

    std::unique_ptr<Title>  mTitle;
    bool mIntroPlayed { false };

    // Main-menu buttons
    std::unique_ptr<Button> mPlayBtn;
    std::unique_ptr<Button> mQuitBtn;

    // Difficulty buttons
    std::unique_ptr<Button> mEasyBtn;
    std::unique_ptr<Button> mMediumBtn;
    std::unique_ptr<Button> mHardBtn;

    //Fonts
    sf::Font hudFont;

    //Backgound and foreground
    sf::Sprite mBg1;
    sf::Texture mTextBg1;
    sf::Sprite mFg1;
    sf::Texture mTextFg1;

    // Decorative star background for menu screens
    sf::Texture mStarTexture;
    sf::Sprite mStarBg;

    Tower mTower;
    WaveHandler mWaves;
    CollisionHandler mCollisionHandler;
    EntityHandler mEntityHandler;
    InputHandler mTowerKeyBindingHandler;


    sf::Music mMusic;
};

#endif