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

    //Load the Font once
    if(!hudFont.loadFromFile("Fonts/Norse.ttf"))
    {
        std::cerr << "Error opening Norse.ttf" << std::endl;
    }
    if(!mTextBackground1.loadFromFile("Sprites/Background.png"))
    {
        std::cerr << "Error opening Background.png" << std::endl;
    }
    mBackground1.setTexture(mTextBackground1);
    if(!mTextForeground1.loadFromFile("Sprites/foreground.png"))
    {
        std::cerr << "Error opening foreground.png" << std::endl;
    }
    mForeground1.setTexture(mTextForeground1);


    sf::Vector2u windowSize  = mWindow.getSize();
    sf::Vector2u backgroundtextureSize = mTextBackground1.getSize();
    mBackground1.setScale(
        static_cast<float>(windowSize.x) / backgroundtextureSize.x,
        static_cast<float>(windowSize.y) / backgroundtextureSize.y
    );

    sf::Vector2u foregroundtextureSize = mTextForeground1.getSize();
    mForeground1.setScale(
        static_cast<float>(windowSize.x) / foregroundtextureSize.x,
        static_cast<float>(windowSize.y) / foregroundtextureSize.y
    );

    //Skill Tree Loading
    mBraverySkillTree = SkillTree("SkillTree/Bravery_Skill_Tree.json");

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
            case State::Controls:
                updateControls(e);
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

                if ((e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::E))
                {
                    mState = State::Playing; //Unpauses the game back to the main game, exiting the magic menu
                    std::cout << "Exited Magic Selection\n";
                    break;
                }
                updateMagicSelection(e);
                break;
            case State::SkillTreeView:
                if (e.type == sf::Event::KeyPressed && 
                    (e.key.code == sf::Keyboard::E || e.key.code == sf::Keyboard::Escape))
                {
                    mState = State::MagicSelection;
                    break;
                }
                updateSkillTreeView(e);
                break;
            case State::GameOver:
                updateGameOver(e);
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
            updateIntro(dt);
            break;
        case State::Playing:
            updatePlaying(dt);
            break;
        case State::GameOver:
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
        case State::Controls:
            renderControls();
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
        case State::GameOver:
            renderGameOver();
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
        sf::Vector2f(centerx, centery - 90.f),
        sf::Vector2f(300.f, 80.f),
        sf::Color(180, 20, 20, 255));

    mControlsBtn = std::make_unique<Button>(
        "Controls",
        sf::Vector2f(centerx, centery + 10.f),
        sf::Vector2f(300.f, 80.f),
        sf::Color(30, 80, 160, 255));

    mQuitBtn = std::make_unique<Button>(
        "Quit",
        sf::Vector2f(centerx, centery + 110.f),
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
    mControlsBtn->update(e, mWindow);
    mQuitBtn->update(e, mWindow);

    // Transition to difficulty select when the Play button's rainbow completes
    if (mPlayBtn->mFading && mPlayBtn->mColorIndex >= mPlayBtn->rainbow.size() - 1)
    {
        initDifficultySelect();
        mState = State::DifficultySelect;
        return;
    }

    // Transition to Controls screen
    if (mControlsBtn->mFading && mControlsBtn->mColorIndex >= mControlsBtn->rainbow.size() - 1)
    {
        initControls();
        mState = State::Controls;
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
    mWindow.draw(*mControlsBtn);
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
    if (!mUIFont.loadFromFile("Fonts/Norse.ttf")) {
    std::cerr << "Failed to load Fonts/Norse.ttf\n";
    }

    mMagicSelection.init(mWindow, mUIFont);
}

void Game::updateMagicSelection(sf::Event& e)
{
    Magic clicked;
    if (mMagicSelection.handleEvent(e, mWindow, clicked))
    {
        mActiveMagic = clicked;
        initSkillTreeView();
        mState = State::SkillTreeView;
    }
}

void Game::renderMagicSelection()
{
    mMagicSelection.draw(mWindow);
}

void Game::initSkillTreeView()
{
    mSkillTreeView.init(mWindow, mUIFont, mBraverySkillTree, mActiveMagic);
}

void Game::updateSkillTreeView(sf::Event& e)
{
std::string clickedId = mSkillTreeView.handleEvent(e, mWindow);
    if (clickedId.empty()) 
    {

        return;
    }
    
    SkillNode* node = mBraverySkillTree.findSkill(clickedId);
    if (!node) 
    {
         std::cout << "findSkill returned null for: " << clickedId << "\n";
        return;
    }
    

    if (node->isUnlocked)
    { 
    return;
    }
    
    if (!mBraverySkillTree.canUnlockSkill(clickedId)) 
    {
        mSkillTreeView.showFailureFeedback("Prerequisites not met!");
        return;
    }
    
    if (mTower.getXPPoints() < node->cost) 
    {
        mSkillTreeView.showFailureFeedback("Insufficient XP!");
        return;
    }

    mTower.spendXP(node->cost);
    mBraverySkillTree.unlockSkill(clickedId);
    applySkillEffect(clickedId);
    std::cout << "Unlocked: " << node->name 
              << " (cost: " << node->cost 
              << ", XP remaining: " << mTower.getXPPoints() << ")\n";
}

void Game::renderSkillTreeView()
{
    mSkillTreeView.draw(mWindow, mTower.getXPPoints());
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
    mTower.reset();
    mWaves.reset();
    mBraverySkillTree.reset();

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
    sf::FloatRect TowerHurtboxBounds = mTower.getHurtboxBounds();
    mTower.update(mWindow, dt);
    mTower.updateAttack(mWindow, dt);
    mWaves.Update(mWindow, dt, TowerHurtboxBounds);
    mCollisionHandler.checkBulletEnemyCollision(mTower.getAttacks(), mWaves);
    mCollisionHandler.checkEnemyTowerCollision(mWaves, mTower);

    // Allows holding mouse to fire continuously
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        mTower.shoot(mWindow);

    for (auto& enemy : mWaves.getEnemies())
    {
        Enemies* enemies = dynamic_cast<Enemies*>(enemy.get());
        if(!enemies) continue;
        if (enemies->isDead())
        {
            enemies->giveXP(mTower);
            std::cout << "Tower XP: " << mTower.getXPPoints() << std::endl;
        }
    }
    if (mTower.getHealth() <= 0)
    {
        initGameOver();
        mState = State::GameOver;
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
    mWindow.draw(mBackground1);
    mWindow.draw(mForeground1);
    mTower.drawAttack(mWindow);
    mWindow.draw(mTower);
    mWaves.DrawEntities(mWindow, sf::RenderStates::Default);
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

void Game::applySkillEffect(const std::string& skillId)
{
    if (skillId == "bravery.fire_manip1.unlock")
    {
        mTower.setDamageMultiplier(1.125f);
        mTower.setBulletDamage(20.f);
        mTower.setBulletSpeed(750.f);
        mTower.setBulletTexture(&mTower.getFireBulletTexture());

    }
    else if (skillId == "bravery.fire_manip2.unlock")
    {
        mTower.setDamageMultiplier(1.25f);
        mTower.setBulletDamage(30.f);
        mTower.setBulletSpeed(850.f);
        mTower.setBulletTexture(&mTower.getFireBulletTexture());

    }
    else if (skillId == "bravery.fire_manip3.unlock")
    {
        mTower.setDamageMultiplier(1.50f);
        mTower.setBulletDamage(40.f);
        mTower.setBulletSpeed(950.f);
        mTower.setBulletTexture(&mTower.getFireBulletTexture());
    }
    else if (skillId == "bravery.fire_manip4.unlock")
    {
        mTower.setDamageMultiplier(1.75f);
        mTower.setBulletDamage(50.f);
        mTower.setBulletSpeed(1050.f);
        mTower.setFireRate(4.0f);
        mTower.setBulletTexture(&mTower.getFireBulletTexture());
    }
}

void Game::initGameOver()
{
    sf::Vector2u windowsize = mWindow.getSize();
    float centerx = windowsize.x / 2.f;
    float centery = windowsize.y / 2.f;

    mRestartBtn = std::make_unique<Button>(
        "Restart",
        sf::Vector2f(centerx, centery + 60.f),
        sf::Vector2f(300.f, 80.f),
        sf::Color(180, 20, 20, 255));

    mMainMenuBtn = std::make_unique<Button>(
        "Main Menu",
        sf::Vector2f(centerx, centery + 160.f),
        sf::Vector2f(300.f, 80.f),
        sf::Color(80, 80, 80, 255));
}

void Game::updateGameOver(sf::Event& e)
{
    mRestartBtn->update(e, mWindow);
    mMainMenuBtn->update(e, mWindow);

    if(mRestartBtn->mFading && 
       mRestartBtn->mColorIndex >= mRestartBtn->rainbow.size() - 1)
    {
        startGame();
        mState = State::Playing;
    }

    if(mMainMenuBtn->mFading && 
       mMainMenuBtn->mColorIndex >= mMainMenuBtn->rainbow.size() - 1)
    {
        initMenu();
        mState = State::Menu;
    }
}

void Game::renderGameOver()
{
    mWindow.draw(mBackground1);
    mWindow.draw(mForeground1);
    sf::Text gameOverText("GAME OVER", mUIFont, 120);
    gameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect bounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    gameOverText.setPosition(
        mWindow.getSize().x / 2.f,
        mWindow.getSize().y / 2.f - 100.f
    );
    mWindow.draw(gameOverText);

    mWindow.draw(*mRestartBtn);
    mWindow.draw(*mMainMenuBtn);
}

void Game::initControls()
{
    sf::Vector2u ws = mWindow.getSize();
    mControlsBackBtn = std::make_unique<Button>(
        "Back",
        sf::Vector2f(ws.x / 2.0f, ws.y - 80.f),
        sf::Vector2f(200.f, 60.f),
        sf::Color(80, 80, 80, 255));
}

/**
 * @brief Check for control page updates
 * 
 * @param e 
 */
void Game::updateControls(sf::Event& e)
{
    mControlsBackBtn->update(e, mWindow);

    if (mControlsBackBtn->mFading && mControlsBackBtn->mColorIndex >= mControlsBackBtn->rainbow.size() - 1)
    {
        initMenu();
        mState = State::Menu;
    }
}

/**
 * @brief Render the controls page
 * 
 */
void Game::renderControls()
{
    sf::Vector2u windowsize = mWindow.getSize();
    float centerx = windowsize.x / 2.0f;

    // title
    sf::Text title("Controls", mUIFont, 48);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.width / 2.f, tb.height / 2.f);
    title.setPosition(centerx, 80.f);
    mWindow.draw(title);

    struct Row { std::string input; std::string action; };
    const std::vector<Row> rows =
    {
        { "Left Click",     "Fire cannon (hold to fire continuously)" },
        { "Mouse Movement", "Aim cannon"                              },
        { "E",              "Open / Close Magic Selection menu"       },
        { "Escape",         "Back / Return to Main Menu"              },
    };

    const float margin = windowsize.x * 0.08f;
    const float tableLeft = margin;
    const float tableWidth = windowsize.x - margin * 2.f;
    const float inputColW = tableWidth * 0.28f;
    const float colLeft = tableLeft;
    const float colRight = tableLeft + inputColW;
    const float rowStart = 200.f;
    const float rowStep = 65.f;
    const unsigned int fontSize = 28;

    auto makeHeader = [&](const std::string& s, float x)
    {
        sf::Text t(s, mUIFont, fontSize + 4);
        t.setFillColor(sf::Color(255, 200, 50, 255));
        t.setStyle(sf::Text::Bold | sf::Text::Underlined);
        t.setPosition(x, rowStart - 55.f);
        mWindow.draw(t);
    };
    makeHeader("Input",  colLeft);
    makeHeader("Action", colRight);


    sf::RectangleShape seperator(sf::Vector2f(tableWidth, 2.f));
    seperator.setFillColor(sf::Color(255, 255, 255, 80));
    seperator.setPosition(tableLeft, rowStart - 12.f);
    mWindow.draw(seperator);

    for (std::size_t i = 0; i < rows.size(); ++i)
    {
        float y = rowStart + i * rowStep;

        if (i % 2 == 0)
        {
            sf::RectangleShape strip(sf::Vector2f(tableWidth, rowStep - 4.f));
            strip.setFillColor(sf::Color(255, 255, 255, 18));
            strip.setPosition(tableLeft, y + 2.f);
            mWindow.draw(strip);
        }

        sf::Text tInput(rows[i].input, mUIFont, fontSize);
        tInput.setFillColor(sf::Color(180, 220, 255, 255));
        tInput.setStyle(sf::Text::Bold);
        tInput.setPosition(colLeft, y + 10.f);
        mWindow.draw(tInput);

        sf::Text tAction(rows[i].action, mUIFont, fontSize);
        tAction.setFillColor(sf::Color::White);
        tAction.setPosition(colRight, y + 10.f);
        mWindow.draw(tAction);
    }


    sf::RectangleShape seperator2(sf::Vector2f(tableWidth, 2.f));
    seperator2.setFillColor(sf::Color(255, 255, 255, 80));
    seperator2.setPosition(tableLeft, rowStart + rows.size() * rowStep);
    mWindow.draw(seperator2);

    mWindow.draw(*mControlsBackBtn);
}