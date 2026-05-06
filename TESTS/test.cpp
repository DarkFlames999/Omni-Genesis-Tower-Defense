/**
 * @file test.cpp
 * @brief Catch2 unit tests for Omni-Genesis / Tower Defense.
 * @author Isaiah Watkins, Keali Lake
 * 
 *
 */
 
#include "catch_amalgamated.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "../Entities/Entities.h"
#include "../Handlers/WaveHandler/WaveHandler.h"
#include "../Handlers/CollisionHandler/CollisionHandler.h"
#include "../Handlers/EntityHandler/EntityHandler.h"
#include "../Handlers/InputHandler/InputHandler.h"
#include "../SkillTree/SkillTree.h"
#include "../GUI-Components/MagicSelection.h"
#include "../DetectionTools/Hitbox.h"
#include "../DetectionTools/Hurtbox.h"
 
using Catch::Approx;
 
// ============================================================================
// Attack
// ============================================================================
TEST_CASE("Attack - default state", "[attack]")
{
    Attack attack;
 
    SECTION("Default attack is alive")
    {
        REQUIRE(attack.isAlive() == true);
    }
 
    SECTION("Default damage is 15")
    {
        REQUIRE(attack.getDamage() == Approx(15.f));
    }
}
 
TEST_CASE("Attack - createAttack initialization", "[attack]")
{
    Attack attack;
    sf::Vector2f spawnPos(100.f, 200.f);
    sf::Vector2f direction(1.f, 0.f);
 
    REQUIRE(attack.createAttack(spawnPos, direction) == true);
    REQUIRE(attack.isAlive() == true);
    REQUIRE(attack.getDamage() == Approx(15.f));
}
 
TEST_CASE("Attack - setters", "[attack]")
{
    Attack attack;
 
    SECTION("setAlive(false) marks attack as dead")
    {
        attack.setAlive(false);
        REQUIRE(attack.isAlive() == false);
    }
 
    SECTION("setDamage updates damage value")
    {
        attack.setDamage(42.f);
        REQUIRE(attack.getDamage() == Approx(42.f));
    }
}
 
TEST_CASE("Attack - computeVisualSize scales with damage", "[attack]")
{
    Attack attack;
 
    attack.setDamage(15.f);
    float baseSize = attack.computeVisualSize();
    REQUIRE(baseSize == Approx(30.f));
 
    attack.setDamage(25.f);
    float largerSize = attack.computeVisualSize();
    REQUIRE(largerSize > baseSize);
    REQUIRE(largerSize == Approx(50.f));
}
 
// ============================================================================
// Tower
// ============================================================================
TEST_CASE("Tower - default state", "[tower]")
{
    Tower tower;
 
    SECTION("Starts at full health")
    {
        REQUIRE(tower.getHealth() == Approx(100.f));
    }
 
    SECTION("Starts with zero XP")
    {
        REQUIRE(tower.getXPPoints() == 0);
    }
 
    SECTION("Default damage multiplier is 1.0")
    {
        REQUIRE(tower.getDamageMultiplier() == Approx(1.0f));
    }
 
    SECTION("Default fire rate is 1.5")
    {
        REQUIRE(tower.getFireRate() == Approx(1.5f));
    }
 
    SECTION("Attacks vector starts empty")
    {
        REQUIRE(tower.getAttacks().empty());
    }
}
 
TEST_CASE("Tower - takeDamage and heal", "[tower]")
{
    Tower tower;
 
    SECTION("takeDamage reduces health")
    {
        tower.takeDamage(25.f);
        REQUIRE(tower.getHealth() == Approx(75.f));
    }
 
    SECTION("Multiple takeDamage calls accumulate")
    {
        tower.takeDamage(20.f);
        tower.takeDamage(30.f);
        REQUIRE(tower.getHealth() == Approx(50.f));
    }
 
    SECTION("heal restores health")
    {
        tower.takeDamage(50.f);
        tower.heal(20.f);
        REQUIRE(tower.getHealth() == Approx(70.f));
    }
 
    SECTION("heal cannot exceed max HP")
    {
        tower.takeDamage(10.f);
        tower.heal(1000.f);
        REQUIRE(tower.getHealth() == Approx(100.f));
    }
}
 
TEST_CASE("Tower - setMaxHP", "[tower]")
{
    Tower tower;
    tower.setMaxHP(200.f);
    tower.heal(1000.f);   // should now be able to climb to the new ceiling
    REQUIRE(tower.getHealth() == Approx(200.f));
}
 
TEST_CASE("Tower - XP economy", "[tower]")
{
    Tower tower;
 
    SECTION("spendXP fails when XP is insufficient")
    {
        REQUIRE(tower.spendXP(10) == false);
        REQUIRE(tower.getXPPoints() == 0);
    }
}
 
TEST_CASE("Tower - stat setters", "[tower]")
{
    Tower tower;
 
    SECTION("setDamageMultiplier updates value")
    {
        tower.setDamageMultiplier(2.5f);
        REQUIRE(tower.getDamageMultiplier() == Approx(2.5f));
    }
 
    SECTION("setFireRate updates value")
    {
        tower.setFireRate(3.0f);
        REQUIRE(tower.getFireRate() == Approx(3.0f));
    }
}
 
TEST_CASE("Tower - reset restores defaults", "[tower]")
{
    Tower tower;
 
    tower.takeDamage(50.f);
    tower.setDamageMultiplier(2.5f);
    tower.setFireRate(5.0f);
 
    tower.reset();
 
    REQUIRE(tower.getHealth() == Approx(100.f));
    REQUIRE(tower.getDamageMultiplier() == Approx(1.0f));
    REQUIRE(tower.getFireRate() == Approx(1.5f));
    REQUIRE(tower.getXPPoints() == 0);
    REQUIRE(tower.getAttacks().empty());
}
 
// ============================================================================
// WaveHandler
// ============================================================================
TEST_CASE("WaveHandler - initial state", "[wave]")
{
    WaveHandler wh;
 
    SECTION("Starts at wave 0")
    {
        REQUIRE(wh.GetCurrentWave() == 0);
    }
 
    SECTION("No enemies initially")
    {
        REQUIRE(wh.getEnemies().empty());
    }
 
    SECTION("Wave is not active before starting")
    {
        REQUIRE(wh.IsWaveActive() == false);
    }
 
    SECTION("IsWaveComplete is false before any wave starts")
    {
        // mAllSpawned is false on construction; complete requires mAllSpawned.
        REQUIRE(wh.IsWaveComplete() == false);
    }
}
 
TEST_CASE("WaveHandler - SetWave", "[wave]")
{
    WaveHandler wh;
    wh.SetWave(5);
    REQUIRE(wh.GetCurrentWave() == 5);
 
    wh.SetWave(20);
    REQUIRE(wh.GetCurrentWave() == 20);
}
 
TEST_CASE("WaveHandler - reset clears state", "[wave]")
{
    WaveHandler wh;
    wh.SetWave(15);
 
    wh.reset();
 
    REQUIRE(wh.GetCurrentWave() == 0);
    REQUIRE(wh.getEnemies().empty());
    REQUIRE(wh.GetTotalEnemiesThisWave() == 0);
    REQUIRE(wh.GetEnemiesRemainingToSpawn() == 0);
}
 
// ============================================================================
// EntityHandler
// ============================================================================
TEST_CASE("EntityHandler - initial state", "[entity]")
{
    EntityHandler eh;
    REQUIRE(eh.getEnemies().empty());
}
 
// ============================================================================
// SkillTree - core API
// ============================================================================
TEST_CASE("SkillTree - empty tree", "[skilltree]")
{
    SkillTree tree;
    REQUIRE(tree.getSkillTreeSize() == 0);
    REQUIRE(tree.findSkill("anything") == nullptr);
}
 
TEST_CASE("SkillTree - addSkillNode and findSkill", "[skilltree]")
{
    SkillTree tree;
 
    SkillNode node;
    node.id = "test.node";
    node.name = "Test Node";
    node.description = "A test node";
    node.cost = 5;
    node.magicType = Magic::Bravery;
    node.upgradeType = UpgradeKind::Offensive;
 
    tree.addSkillNode(node);
    REQUIRE(tree.getSkillTreeSize() == 1);
 
    SkillNode* found = tree.findSkill("test.node");
    REQUIRE(found != nullptr);
    REQUIRE(found->name == "Test Node");
    REQUIRE(found->cost == 5);
    REQUIRE(found->isUnlocked == false);
 
    REQUIRE(tree.findSkill("missing") == nullptr);
}
 
TEST_CASE("SkillTree - unlocking with no prerequisites", "[skilltree]")
{
    SkillTree tree;
 
    SkillNode root;
    root.id = "root";
    root.name = "Root";
    root.cost = 1;
    root.magicType = Magic::Bravery;
    root.upgradeType = UpgradeKind::Offensive;
    tree.addSkillNode(root);
 
    REQUIRE(tree.canUnlockSkill("root") == true);
    tree.unlockSkill("root");
    REQUIRE(tree.findSkill("root")->isUnlocked == true);
}
 
TEST_CASE("SkillTree - cannot re-unlock an already unlocked skill", "[skilltree]")
{
    SkillTree tree;
 
    SkillNode root;
    root.id = "root";
    root.cost = 1;
    root.magicType = Magic::Bravery;
    root.upgradeType = UpgradeKind::Offensive;
    tree.addSkillNode(root);
 
    tree.unlockSkill("root");
    REQUIRE(tree.canUnlockSkill("root") == false);
}
 
TEST_CASE("SkillTree - prerequisites gate unlocking", "[skilltree]")
{
    SkillTree tree;
 
    SkillNode root;
    root.id = "root";
    root.cost = 1;
    root.magicType = Magic::Bravery;
    root.upgradeType = UpgradeKind::Offensive;
 
    SkillNode child;
    child.id = "child";
    child.cost = 2;
    child.magicType = Magic::Bravery;
    child.upgradeType = UpgradeKind::Offensive;
    child.prerequisites = { "root" };
 
    tree.addSkillNode(root);
    tree.addSkillNode(child);
 
    SECTION("Child cannot be unlocked while root is locked")
    {
        REQUIRE(tree.canUnlockSkill("child") == false);
 
        tree.unlockSkill("child");                        // should silently fail
        REQUIRE(tree.findSkill("child")->isUnlocked == false);
    }
 
    SECTION("Child becomes unlockable once root is unlocked")
    {
        tree.unlockSkill("root");
        REQUIRE(tree.canUnlockSkill("child") == true);
 
        tree.unlockSkill("child");
        REQUIRE(tree.findSkill("child")->isUnlocked == true);
    }
}
 
TEST_CASE("SkillTree - canUnlockSkill returns false for unknown id", "[skilltree]")
{
    SkillTree tree;
    REQUIRE(tree.canUnlockSkill("does_not_exist") == false);
}
 
TEST_CASE("SkillTree - reset locks all skills", "[skilltree]")
{
    SkillTree tree;
 
    SkillNode a;
    a.id = "a"; a.cost = 1;
    a.magicType = Magic::Bravery;
    a.upgradeType = UpgradeKind::Offensive;
 
    SkillNode b;
    b.id = "b"; b.cost = 1;
    b.magicType = Magic::Bravery;
    b.upgradeType = UpgradeKind::Offensive;
 
    tree.addSkillNode(a);
    tree.addSkillNode(b);
 
    tree.unlockSkill("a");
    tree.unlockSkill("b");
    REQUIRE(tree.findSkill("a")->isUnlocked == true);
    REQUIRE(tree.findSkill("b")->isUnlocked == true);
 
    tree.reset();
    REQUIRE(tree.findSkill("a")->isUnlocked == false);
    REQUIRE(tree.findSkill("b")->isUnlocked == false);
}
 
// ============================================================================
// SkillTree - parsing helpers
// ============================================================================
TEST_CASE("SkillTree - parseMagic handles all magic names case-insensitively", "[skilltree][parse]")
{
    SkillTree tree;
 
    REQUIRE(tree.parseMagic("Determination") == Magic::Determination);
    REQUIRE(tree.parseMagic("BRAVERY") == Magic::Bravery);
    REQUIRE(tree.parseMagic("justice") == Magic::Justice);
    REQUIRE(tree.parseMagic("Kindness") == Magic::Kindness);
    REQUIRE(tree.parseMagic("Patience") == Magic::Patience);
    REQUIRE(tree.parseMagic("Integrity") == Magic::Integrity);
    REQUIRE(tree.parseMagic("Perseverance") == Magic::Perseverance);
    REQUIRE(tree.parseMagic("Apathy") == Magic::Apathy);
    REQUIRE(tree.parseMagic("Fear") == Magic::Fear);
    REQUIRE(tree.parseMagic("Chaos") == Magic::Chaos);
    REQUIRE(tree.parseMagic("Wrath") == Magic::Wrath);
    REQUIRE(tree.parseMagic("Nihilistic") == Magic::Nihilistic);
    REQUIRE(tree.parseMagic("Deceit") == Magic::Deceit);
    REQUIRE(tree.parseMagic("Irresolution") == Magic::Irresolution);
}
 
TEST_CASE("SkillTree - parseUpgradeKind handles all kinds case-insensitively", "[skilltree][parse]")
{
    SkillTree tree;
 
    REQUIRE(tree.parseUpgradeKind("Offensive") == UpgradeKind::Offensive);
    REQUIRE(tree.parseUpgradeKind("DEFENSIVE") == UpgradeKind::Defensive);
    REQUIRE(tree.parseUpgradeKind("utility")   == UpgradeKind::Utility);
}
 
// ============================================================================
// MagicIcon
// ============================================================================
TEST_CASE("MagicIcon - contains hit-tests around its center", "[magic]")
{
    MagicIcon icon;
    icon.position = sf::Vector2f(100.f, 100.f);
    icon.size     = sf::Vector2f(50.f, 50.f);
    // bounds = (75,75) -> (125,125)
 
    SECTION("Center point is inside")
    {
        REQUIRE(icon.contains(sf::Vector2f(100.f, 100.f)) == true);
    }
 
    SECTION("Edge point is inside")
    {
        REQUIRE(icon.contains(sf::Vector2f(80.f, 100.f)) == true);
    }
 
    SECTION("Far-away point is outside")
    {
        REQUIRE(icon.contains(sf::Vector2f(200.f, 200.f)) == false);
    }
 
    SECTION("Negative-coordinate point is outside")
    {
        REQUIRE(icon.contains(sf::Vector2f(-10.f, -10.f)) == false);
    }
}
 
// ============================================================================
// InputHandler
// ============================================================================
TEST_CASE("InputHandler - bindings start empty", "[input]")
{
    InputHandler ih;
    REQUIRE(ih.mHeldKeyBindings.empty());
    REQUIRE(ih.mDoubleTapBindings.empty());
}
 
TEST_CASE("InputHandler - ImplementHeldKey registers a binding", "[input]")
{
    InputHandler ih;
    ih.ImplementHeldKey(sf::Keyboard::Space, 1.0f);
 
    REQUIRE(ih.mHeldKeyBindings.size() == 1);
    REQUIRE(ih.mHeldKeyBindings[0].key == sf::Keyboard::Space);
    REQUIRE(ih.mHeldKeyBindings[0].timeToHold == Approx(1.0f));
    REQUIRE(ih.mHeldKeyBindings[0].isHolding == false);
    REQUIRE(ih.mHeldKeyBindings[0].holdAchieved == false);
}
 
TEST_CASE("InputHandler - ImplementDoubleTap registers a binding", "[input]")
{
    InputHandler ih;
    ih.ImplementDoubleTap(sf::Keyboard::E, 0.3f);
 
    REQUIRE(ih.mDoubleTapBindings.size() == 1);
    REQUIRE(ih.mDoubleTapBindings[0].key == sf::Keyboard::E);
    REQUIRE(ih.mDoubleTapBindings[0].timeToDoubleTap == Approx(0.3f));
    REQUIRE(ih.mDoubleTapBindings[0].firstTap == false);
}
 
TEST_CASE("InputHandler - first KeyPressed flips firstTap on a double-tap binding", "[input]")
{
    InputHandler ih;
    ih.ImplementDoubleTap(sf::Keyboard::E, 0.3f);
 
    sf::Event ev{};
    ev.type = sf::Event::KeyPressed;
    ev.key.code = sf::Keyboard::E;
 
    ih.update(ev);
 
    REQUIRE(ih.mDoubleTapBindings[0].firstTap == true);
    REQUIRE(ih.mDoubleTapBindings[0].tapCount == Approx(1.0f));
}
 
TEST_CASE("InputHandler - KeyPressed flips isHolding on a held-key binding", "[input]")
{
    InputHandler ih;
    ih.ImplementHeldKey(sf::Keyboard::Space, 1.0f);
 
    sf::Event ev{};
    ev.type = sf::Event::KeyPressed;
    ev.key.code = sf::Keyboard::Space;
 
    ih.update(ev);
 
    REQUIRE(ih.mHeldKeyBindings[0].isHolding == true);
}
 
// ============================================================================
// Hitbox / Hurtbox
// ============================================================================
TEST_CASE("Hitbox - global bounds reflect size and position", "[hitbox]")
{
    Hitbox hb;
    hb.setSize(50.f, 40.f);
    hb.setPosition(100.f, 200.f);
 
    sf::FloatRect bounds = hb.getGlobalBounds();
    REQUIRE(bounds.width  == Approx(50.f));
    REQUIRE(bounds.height == Approx(40.f));
    REQUIRE(bounds.left   == Approx(100.f));
    REQUIRE(bounds.top    == Approx(200.f));
}
 
TEST_CASE("Hurtbox - global bounds reflect size and position", "[hurtbox]")
{
    Hurtbox hb;
    hb.setSize(60.f, 80.f);
    hb.setPosition(50.f, 70.f);
 
    sf::FloatRect bounds = hb.getGlobalBounds();
    REQUIRE(bounds.width  == Approx(60.f));
    REQUIRE(bounds.height == Approx(80.f));
    REQUIRE(bounds.left   == Approx(50.f));
    REQUIRE(bounds.top    == Approx(70.f));
}
 
TEST_CASE("Hitbox vs Hurtbox - intersection detection", "[collision]")
{
    Hitbox bullet;
    bullet.setSize(20.f, 20.f);
    bullet.setPosition(100.f, 100.f);
 
    Hurtbox enemy;
    enemy.setSize(40.f, 40.f);
 
    SECTION("Overlapping boxes intersect")
    {
        enemy.setPosition(110.f, 110.f);
        REQUIRE(bullet.getGlobalBounds().intersects(enemy.getGlobalBounds()));
    }
 
    SECTION("Far-apart boxes do not intersect")
    {
        enemy.setPosition(1000.f, 1000.f);
        REQUIRE_FALSE(bullet.getGlobalBounds().intersects(enemy.getGlobalBounds()));
    }
}