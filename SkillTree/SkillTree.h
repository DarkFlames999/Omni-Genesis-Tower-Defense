/**
 * @file SkillTree.h
 * @author Isaiah Watkins, Konner Knoll, and Keali Lake
 * @brief SkillTree class declaration
 * @version 0.1
 * @date 2026-04-22
 */
#ifndef SKILLTREE_H
#define SKILLTREE_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "../Entities/Entities.h"
#include <nlohmann/json.hpp> // For my json file handling... reminder to ask others to install it
#include <SFML/Graphics.hpp>
// #include "../GUI-Components/menu.h"

enum class Magic{ //To help us decide what skill tree it goes under
    Determination, Bravery, Justice, Kindness, Patience, Integrity, Perseverance,
    Apathy, Fear, Chaos, Wrath, Nihilistic, Deceit, Irresolution 
};

enum class UpgradeKind { //To help us decide what type of upgrade it is
    Offensive, Defensive, Utility
};

struct SkillNode {
    std::string id;
    std::string name;
    std::string description;
    bool isUnlocked;
    Magic magicType;
    UpgradeKind upgradeType;
    int cost = 1;
    std::vector<std::string> skillpathway; // I.E. "Thermal Energy Manipulation" -> "Fire Manipulation"
    std::vector<std::string> prerequisites; // List of skill node IDs that are prerequisites

    // Button skillNodeShape; // For the visual representation of the skill node in the skill tree UI
};

class SkillTree {
public:
    SkillTree(std::string filename);
    ~SkillTree();
    void addSkillNode(const SkillNode& node);
    //WIll need to add function to red Json file to load skill tree from file, and function to save skill tree to file
    void loadFromJson(const std::string& filename);
    void unlockSkill(const std::string& skillId);
    bool canUnlockSkill(const std::string& skillId);

    //helper function for JSON conversation
    Magic parseMagic(const std::string& magictype);
    UpgradeKind parseUpgradeKind(const std::string& upgradekind);

protected:
    std::vector <SkillNode> skillNodes;

private:

};
#endif//SKILLTREE_H