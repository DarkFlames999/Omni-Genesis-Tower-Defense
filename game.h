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
#include "GUI-Components/MagicSelection.h"
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
    enum class State       { Intro, Menu, DifficultySelect, Playing, MagicSelection, SkillTreeView, Paused };
    enum class Difficulty  { None, Easy, Medium, Hard, Areyousure };

    void processEvents();
    void update(float dt);
    void render();

    void updateIntro(float dt);
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
    std::unique_ptr<Button> mAreyousureBtn;

    // Decorative star background for menu screens
    sf::Texture mStarTexture;
    sf::Sprite mStarBg;
    sf::Texture mSkillTreeBgTexture;
    sf::Texture mSkillTreeFgTexture;


    Tower mTower;
    WaveHandler mWaves;
    CollisionHandler mCollisionHandler;
    EntityHandler mEntityHandler;
    InputHandler mTowerKeyBindingHandler;
    SkillTree mBraverySkillTree;
    Magic mActiveMagic { Magic::Bravery };
    MagicSelection mMagicSelection;
    
    void initMagicSelection();
    void updateMagicSelection(sf::Event& e);
    void renderMagicSelection();

    void initSkillTreeView();
    void updateSkillTreeView(sf::Event& e);
    void renderSkillTreeView();

    sf::SoundBuffer mMusicBuffer;
    sf::Sound mMusic;
    sf::Font mUIFont;
};

#endif