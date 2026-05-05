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

    // Background music
    if (!mMusic.openFromFile("music/background.ogg")) {
        std::cerr << "Warning: failed to load background music\n";
    }
    mMusic.setLoop(true);
    mMusic.setVolume(50.f);
    mMusic.play();

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
                break;
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
            updateIntro();
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
        default:
            break;
    }

    mWindow.display();
}

/**
 * @brief Intro state
 * 
 */
void Game::updateIntro()
{
    mTitle->update(mWindow);

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
    float cx = ws.x / 2.0f;
    float cy = ws.y / 2.0f;

    mPlayBtn = std::make_unique<Button>(
        "Play",
        sf::Vector2f(cx, cy - 60.f),
        sf::Vector2f(300.f, 80.f),
        sf::Color(180, 20, 20, 255));

    mQuitBtn = std::make_unique<Button>(
        "Quit",
        sf::Vector2f(cx, cy + 60.f),
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
    sf::Vector2u ws = mWindow.getSize();
    float cx = ws.x / 2.0f;
    float cy = ws.y / 2.0f;

    mEasyBtn = std::make_unique<Button>(
        "Easy",
        sf::Vector2f(cx, cy - 100.f),
        sf::Vector2f(300.f, 80.f),
        sf::Color(30, 160, 30, 255));

    mMediumBtn = std::make_unique<Button>(
        "Medium",
        sf::Vector2f(cx, cy),
        sf::Vector2f(300.f, 80.f),
        sf::Color(200, 150, 0, 255));

    mHardBtn = std::make_unique<Button>(
        "Hard",
        sf::Vector2f(cx, cy + 100.f),
        sf::Vector2f(300.f, 80.f),
        sf::Color(180, 20, 20, 255));
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
}

/**
 * @brief Render difficulty selection state
 * 
 */
void Game::renderDifficultySelect()
{
    mTitle->draw(mWindow);

    // "Select Difficulty" label
    sf::Font labelFont;
    labelFont.loadFromFile("Fonts/Norse.ttf");
    sf::Text label("Select Difficulty", labelFont, 36);
    label.setFillColor(sf::Color::White);
    sf::FloatRect lb = label.getLocalBounds();
    label.setOrigin(lb.width / 2.f, lb.height / 2.f);
    sf::Vector2u ws = mWindow.getSize();
    label.setPosition(ws.x / 2.f, ws.y / 2.f - 200.f);
    mWindow.draw(label);

    mWindow.draw(*mEasyBtn);
    mWindow.draw(*mMediumBtn);
    mWindow.draw(*mHardBtn);
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
        default:
            startWave = 0;
            break;
    }

    mWaves.SetWave(startWave);
    mWaves.StartNextWave(mWindow);
}

/**
 * @brief Update playing state
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
    sf::Font hudFont;
    hudFont.loadFromFile("Fonts/Norse.ttf");
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
        default:
            diffStr = "";
            break;
    }
    sf::Text hudText("Difficulty: " + diffStr, hudFont, 22);
    hudText.setFillColor(sf::Color::White);
    hudText.setPosition(mWindow.getSize().x - 220.f, 10.f);
    mWindow.draw(hudText);
}