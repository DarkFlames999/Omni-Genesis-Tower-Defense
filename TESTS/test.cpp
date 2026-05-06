#include "../catch_amalgamated.hpp"
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
#include "game.h"

TEST_CASE("WaveHandler - Wave Initialization")
{
    WaveHandler waveHandler;
    SECTION("Wave Initialization")
    {
        REQUIRE(waveHandler.getCurrentWave() = 1);
    }
    SECTION("Enemy Spawning")
    {
        waveHandler.setWave();
        REQUIRE(waveHandler.getEnemies().size() > 0);
    }
}

TEST_CASE("CollisionHandler - Bullet and Enemy Collision") 
{
    Bullet bullet(sf::Vector2f(0, 0), sf::Vector2f(1, 0));
    Enemy enemy(sf::Vector2f(0, 0));
    Tower tower(sf::Vector2f(0, 0));
    CollisionHandler collisionHandler;
    SECTION("Bullet and Enemy Collision")
    {
        REQUIRE(collisionHandler.checkBulletEnemyCollision(bullet, enemy));
    }
    SECTION("Enemy and Tower Collision")
    {
        REQUIRE(collisionHandler.checkEnemyTowerCollision(enemy, tower));
    }
}

TEST_CASE("Enemy - Initialization")
{
    Enemy enemy(sf::Vector2f(0, 0));
    REQUIRE(enemy.getPosition() == sf::Vector2f(0, 0));
}

TEST_CASE("Tower - Shooting") 
{
    Tower tower(sf::Vector2f(0, 0));
    tower.shoot();
    REQUIRE(tower.getAttacks().size() == 1);
}

TEST_CASE("SkillTree - Skill Unlocking") 
{
    SkillTree skillTree;
    skillTree.unlockSkill("Fireball");
    REQUIRE(skillTree.isSkillUnlocked("Fireball"));
}

TEST_CASE("SkillTree - Skill Effect Application") 
{
    SkillTree skillTree;
    Tower tower(sf::Vector2f(0, 0));
    skillTree.applySkillEffect(tower, "Fireball");
    REQUIRE(tower.getAttackDamage() > 10); // Example check
}

TEST_CASE("InputHandler - Mouse Click Handling") 
{
    InputHandler inputHandler;
    sf::Event event;
    event.type = sf::Event::MouseButtonPressed;
    event.mouseButton.button = sf::Mouse::Left;
    REQUIRE(inputHandler.handleMouseClick(event));
}

TEST_CASE("Menu - Button Click Handling") 
{
    Menu menu;
    sf::Event event;
    event.type = sf::Event::MouseButtonPressed;
    event.mouseButton.button = sf::Mouse::Left;
    REQUIRE(menu.handleButtonClick(event));
}

TEST_CASE("MagicSelection - Spell Selection") 
{
    MagicSelection magicSelection;
    magicSelection.selectSpell("Fireball");
    REQUIRE(magicSelection.getSelectedSpell() == "Fireball");
}