/**
 * @file game.cpp
 * @author Konner Knoll
 * @brief Game class definitions
 * @version 0.1
 */
#include "game.h"
#include <iostream>

/**
 * @brief Construct a new Game:: Game object
 * 
 */
Game::Game()
    : mWindow(sf::VideoMode::getDesktopMode(),
              "Omni-Genesis / Tower Defense",
              sf::Style::Fullscreen)
{
    mWindow.setFramerateLimit(60);

    //Skill Tree Loading
    mBraverySkillTree = SkillTree("SkillTree/Bravery_Skill_Tree.json");
    std::cout << "Loaded " << mBraverySkillTree.getSkillTreeSize() << " skills\n";

    // Background music
    if (!mMusicBuffer.loadFromFile("music/background.ogg")) {
        std::cerr << "Warning: failed to load background music\n";
    }
    mMusic.setBuffer(mMusicBuffer);

    // Font
    if (!mUIFont.loadFromFile("Fonts/Norse.ttf"))
    {
        std::cerr << "Warning: failed to load UI font\n";
    }

    //mMusic.setLoop(true);
    //mMusic.setVolume(50.f);
    //mMusic.play();

    // The intro runs first
    mTitle = std::make_unique<Title>("Omni-Genesis/Tower Defense");
}

/**
 * @brief Run the main game loop
 * 
 */
void Game::run()
{
    starAnimation(mWindow);

    while (mWindow.isOpen())
    {
        float dt = mClock.restart().asSeconds();
        if (dt > 0.1f) {
            dt = 0.1f;
        }

        processEvents();
        update(dt);
        render();
    }
}

/**
 * @brief Event processing based on state
 * 
 */
void Game::processEvents()
{
    sf::Event e;
    while (mWindow.pollEvent(e))
    {
        if (e.type == sf::Event::Closed) {
            mWindow.close();
        }
        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
        {
            if (mState == State::Playing || mState == State::Paused) {
                mState = State::Menu;
                initMenu();
            } else if (mState == State::MagicSelection) {
                mState = State::Playing;
            } else if (mState == State::SkillTreeView) {
                mState = State::MagicSelection;
            } else {
                mWindow.close();
            }
        }

        switch (mState)
        {
            case State::Menu:
                updateMenu(e);
                break;
            case State::DifficultySelect:
                updateDifficultySelect(e);
                break;
            case State::Playing:
                if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
                {
                    mTower.shoot(mWindow);
                }
                if ((e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::E))
                {
                    initMagicSelection();
                    mState = State::MagicSelection;
                    std::cout << "Entered Magic Selection\n";
                }
                break;

            case State::MagicSelection:
                std::cout << "[render dispatch] state is MagicSelection\n";
                if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
                {
                    mState = State::Playing; //Unpauses the game back to the main game, exiting the magic menu
                    std::cout << "Exited Magic Selection\n";
                    break;
                }
                updateMagicSelection(e);
                break;
            case State::SkillTreeView:
                if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) //Allows us to exit the menu back
                {
                    mState = State::MagicSelection;
                    std::cout << "Exited Skill Tree View\n";
                    break;
                }
            default:
                break;
        }
    }
}

/**
 * @brief Update game based on state
 * 
 * @param dt 
 */
void Game::update(float dt)
{
    switch (mState)
    {
        case State::Intro:
            updateIntro(dt);
            break;
        case State::Playing:
            updatePlaying(dt);
            break;
        default:
            break;
    }
}

/**
 * @brief Render game based on state
 * 
 */
void Game::render()
{
    mWindow.clear(sf::Color::Black);

    switch (mState)
    {
        case State::Intro:
            renderIntro();
            break;
        case State::Menu:
            renderMenu();
            break;
        case State::DifficultySelect:
            renderDifficultySelect();
            break;
        case State::Playing:
            renderPlaying();
            break;
        case State::MagicSelection:
            renderMagicSelection();
            break;
        case State::SkillTreeView:
            renderSkillTreeView();
            break;
        default:
            break;
    }

    mWindow.display();
}

/**
 * @brief Intro state
 * 
 */
void Game::updateIntro(float dt)
{
    mTitle->update(mWindow, dt);

    if (mTitle->titleComplete && mTitle->titleSize <= 20.0f)
    {
        initMenu();
        mState = State::Menu;
    }
}

/**
 * @brief Render intro
 * 
 */
void Game::renderIntro()
{
    mTitle->draw(mWindow);
}

/**
 * @brief Menu state
 * 
 */
void Game::initMenu()
{
    sf::Vector2u ws = mWindow.getSize();
    float centerx = ws.x / 2.0f;
    float centery = ws.y / 2.0f;

    mPlayBtn = std::make_unique<Button>(
        "Play",
        sf::Vector2f(centerx, centery - 60.f),
        sf::Vector2f(300.f, 80.f),
        sf::Color(180, 20, 20, 255));

    mQuitBtn = std::make_unique<Button>(
        "Quit",
        sf::Vector2f(centerx, centery + 60.f),
        sf::Vector2f(300.f, 80.f),
        sf::Color(80, 80, 80, 255));
}

/**
 * @brief Update menu state
 * 
 * @param e 
 */
void Game::updateMenu(sf::Event& e)
{
    mPlayBtn->update(e, mWindow);
    mQuitBtn->update(e, mWindow);

    // Transition to difficulty select when the Play button's rainbow completes
    if (mPlayBtn->mFading && mPlayBtn->mColorIndex >= mPlayBtn->rainbow.size() - 1)
    {
        initDifficultySelect();
        mState = State::DifficultySelect;
        return;
    }

    // Quit
    if (mQuitBtn->mFading && mQuitBtn->mColorIndex >= mQuitBtn->rainbow.size() - 1) {
        mWindow.close();
    }
}

/**
 * @brief Render menu state
 * 
 */
void Game::renderMenu()
{
    mTitle->draw(mWindow);
    mWindow.draw(*mPlayBtn);
    mWindow.draw(*mQuitBtn);
}

/**
 * @brief Difficulty selection state
 * 
 */
void Game::initDifficultySelect()
{
    sf::Vector2u windowsize = mWindow.getSize();
    float centerx = windowsize.x / 2.0f;
    float centery = windowsize.y / 2.0f;

    mEasyBtn = std::make_unique<Button>(
        "Easy",
        sf::Vector2f(centerx, centery - 100.f),
        sf::Vector2f(300.f, 80.f),
        sf::Color(30, 160, 30, 255));

    mMediumBtn = std::make_unique<Button>(
        "Medium",
        sf::Vector2f(centerx, centery),
        sf::Vector2f(300.f, 80.f),
        sf::Color(200, 150, 0, 255));

    mHardBtn = std::make_unique<Button>(
        "Hard",
        sf::Vector2f(centerx, centery + 100.f),
        sf::Vector2f(300.f, 80.f),
        sf::Color(180, 20, 20, 255));

    mAreyousureBtn = std::make_unique<Button>(
        "Are you sure?",
        sf::Vector2f(centerx, centery + 200.f),
        sf::Vector2f(300.f, 80.f),
        sf::Color(180, 20, 20, 255));
}

void Game::initMagicSelection()
{
    // Placeholder for magic selection initialization
        std::cout << "[initMagicSelection] called\n";
}

void Game::updateMagicSelection(sf::Event& e)
{
    // Placeholder for magic selection update logic
    //Just for testing before I add buttons for the magic selection
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::B)
    {
        mActiveMagic = Magic::Bravery;
        initSkillTreeView();
        mState = State::SkillTreeView;
    }
}

void Game::renderMagicSelection()
{
    // Placeholder for magic selection rendering
        // Dim background
            std::cout << "[renderMagicSelection] called\n";
    sf::RectangleShape dimbg(sf::Vector2f(mWindow.getSize()));
    dimbg.setFillColor(sf::Color(0, 0, 0, 0));
    dimbg.setTexture(&mSkillTreeBgTexture);
    mWindow.draw(dimbg);
    sf::RectangleShape dimfg(sf::Vector2f(mWindow.getSize()));
    dimfg.setFillColor(sf::Color(255, 255, 255, 0));
    dimfg.setTexture(&mSkillTreeFgTexture);
    mWindow.draw(dimfg);
    sf::Text text("MAGIC SELECTION (press B for Bravery, E to close)", mUIFont, 36);
    text.setFillColor(sf::Color::Yellow);
    sf::FloatRect b = text.getLocalBounds();
    text.setOrigin(b.width / 2.f, b.height / 2.f);
    text.setPosition(mWindow.getSize().x / 2.f, mWindow.getSize().y / 2.f);
    mWindow.draw(text);
}

void Game::initSkillTreeView()
{
    // Placeholder for skill tree view initialization
}

void Game::updateSkillTreeView(sf::Event& e)
{
    // Placeholder for skill tree view update logic
}

void Game::renderSkillTreeView()
{
    // Placeholder for skill tree view rendering
    // Dim background
    sf::RectangleShape dimbg(sf::Vector2f(mWindow.getSize()));
    dimbg.setFillColor(sf::Color(0, 0, 0, 0));
    dimbg.setTexture(&mSkillTreeBgTexture);
    mWindow.draw(dimbg);
    sf::RectangleShape dimfg(sf::Vector2f(mWindow.getSize()));
    dimfg.setFillColor(sf::Color(255, 255, 255, 0));
    dimfg.setTexture(&mSkillTreeFgTexture);
    mWindow.draw(dimfg);

    sf::Text text("SKILL TREE VIEW (press E to close)", mUIFont, 36);
    text.setFillColor(sf::Color::White);
    sf::FloatRect b = text.getLocalBounds();
    text.setOrigin(b.width / 2.f, b.height / 2.f);
    text.setPosition(mWindow.getSize().x / 2.f, mWindow.getSize().y / 2.f);
    mWindow.draw(text);
}

/**
 * @brief Update difficulty selection state
 * 
 * @param e 
 */
void Game::updateDifficultySelect(sf::Event& e)
{
    mEasyBtn->update(e, mWindow);
    mMediumBtn->update(e, mWindow);
    mHardBtn->update(e, mWindow);
    mAreyousureBtn->update(e, mWindow);

    auto chosen = [&](Button& btn, Difficulty d)
    {
        if (btn.mFading && btn.mColorIndex >= btn.rainbow.size() - 1)
        {
            mDifficulty = d;
            startGame();
            mState = State::Playing;
        }
    };

    chosen(*mEasyBtn, Difficulty::Easy);
    chosen(*mMediumBtn, Difficulty::Medium);
    chosen(*mHardBtn, Difficulty::Hard);
    chosen(*mAreyousureBtn, Difficulty::Areyousure);
}

/**
 * @brief Render difficulty selection state
 * 
 */
void Game::renderDifficultySelect()
{
    mTitle->draw(mWindow);

    // "Select Difficulty" label
    sf::Text label("Select Difficulty", mUIFont, 36);
    label.setFillColor(sf::Color::White);
    sf::FloatRect lb = label.getLocalBounds();
    label.setOrigin(lb.width / 2.f, lb.height / 2.f);
    sf::Vector2u ws = mWindow.getSize();
    label.setPosition(ws.x / 2.f, ws.y / 2.f - 200.f);
    mWindow.draw(label);

    mWindow.draw(*mEasyBtn);
    mWindow.draw(*mMediumBtn);
    mWindow.draw(*mHardBtn);
    mWindow.draw(*mAreyousureBtn);
}

/**
 * @brief Playing state
 * 
 */
void Game::startGame()
{
    mTower.createTower(mWindow, {40.0f, 130.0f}, {10.f, 10.f});

    int startWave = 0;
    switch (mDifficulty)
    {
        case Difficulty::Easy:
            startWave = 0;
            break;
        case Difficulty::Medium:
            startWave = 10;
            break;
        case Difficulty::Hard:
            startWave = 20;
            break;
        case Difficulty::Areyousure:
            startWave = 30;
            break;
        default:
            startWave = 0;
            break;
    }

    mWaves.SetWave(startWave);
    mWaves.StartNextWave(mWindow);
}

/**
 * @brief Update gameplay
 * 
 * @param dt 
 */
void Game::updatePlaying(float dt)
{
    mTower.update(mWindow, dt);
    mTower.updateAttack(mWindow, dt);
    mWaves.Update(mWindow, dt);
    mCollisionHandler.checkBulletEnemyCollision(mTower.getAttacks(), mWaves);
    mCollisionHandler.checkEnemyTowerCollision(mWaves, mTower);

    // Allows holding mouse to fire continuously
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        mTower.shoot(mWindow);

    // Grant XP for kills
    for (auto& enemy : mEntityHandler.getEnemies())
    {
        Enemies* enemies = dynamic_cast<Enemies*>(enemy.get());
        if (enemies->isDead())
        {
            enemies->giveXP(mTower);
            std::cout << "Tower XP: " << mTower.getXPPoints() << std::endl;
        }
    }

    if (mTower.getHealth() <= 0)
    {
        // Lose condition for later
    }

    if (mWaves.IsWaveComplete()) {
        mWaves.StartNextWave(mWindow);
    }
}


/**
 * @brief Render playing state
 * 
 */
void Game::renderPlaying()
{
    mWindow.draw(mTower);
    mTower.drawAttack(mWindow);
    mWaves.DrawEntities(mWindow, sf::RenderStates::Default);

    // show current difficulty in top-right
    std::string diffStr;
    switch (mDifficulty)
    {
        case Difficulty::Easy:
            diffStr = "Easy";
            break;
        case Difficulty::Medium:
            diffStr = "Medium";
            break;
        case Difficulty::Hard:
            diffStr = "Hard";
            break;
        case Difficulty::Areyousure:
            diffStr = "Are you sure?";
            break;
        default:
            diffStr = "";
            break;
    }
    sf::Text hudText("Difficulty: " + diffStr, mUIFont, 22);
    hudText.setFillColor(sf::Color::White);
    hudText.setPosition(mWindow.getSize().x - 220.f, 10.f);
    mWindow.draw(hudText);
}