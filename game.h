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
#include "SkillTree/SkillTreeView.h"
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
    enum class State { Intro, Menu, DifficultySelect, Playing, MagicSelection, SkillTreeView, Paused, Controls };

    enum class Difficulty { None, Easy, Medium, Hard, Areyousure };

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

    void initControls();
    void updateControls(sf::Event& e);
    void renderControls();

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

    std::unique_ptr<Button> mControlsBtn;
    std::unique_ptr<Button> mControlsBackBtn;

    //Fonts
    sf::Font hudFont;
    sf::Font mSkillTreeFont;
    //Backgound and foreground
    sf::Sprite mBackground1;
    sf::Texture mTextBackground1;
    sf::Sprite mForeground1;
    sf::Texture mTextForeground1;
    // Decorative star background for menu screens
    sf::Texture mStarTexture;
    sf::Sprite mStarBackground;
    Tower mTower;
    WaveHandler mWaves;
    CollisionHandler mCollisionHandler;
    EntityHandler mEntityHandler;
    InputHandler mTowerKeyBindingHandler;
    //Skill Tree Stuff - DON'T TOUCH PLEAS FOR THE LOVE OF GOD
    SkillTree mBraverySkillTree;
    Magic mActiveMagic { Magic::Bravery };
    MagicSelection mMagicSelection;
    SkillTreeView mSkillTreeView;
    void initMagicSelection();
    void updateMagicSelection(sf::Event& e);
    void renderMagicSelection();
    void initSkillTreeView();
    void updateSkillTreeView(sf::Event& e);
    void renderSkillTreeView();
    void applySkillEffect(const std::string& skillId);
    sf::SoundBuffer mMusicBuffer;
    sf::Sound mMusic;
    sf::Font mUIFont;
};

#endif