/**
 * @file SkillTree.cpp
 * @author Isaiah Watkins, Konner Knoll, and Keali Lake
 * @brief SkillTree class implementation
 * @version 0.1
 * @date 2026-04-22
 */
#include "SkillTree.h"

SkillTree::SkillTree(std::string filename) {
    // Load skill nodes from a JSON file
    loadFromJson(filename); //Upon initialization, loads the particular skill tree from a JSON file

    skillNodeShape.setSize(sf::Vector2f(50, 50)); // Set the size of each skill node shape
    skillNodeShape.setFillColor(sf::Color(255,165,0)); // Set the color of the skill node shape
}

void SkillTree::addSkillNode(const SkillNode& node) {
    skillNodes.push_back(node);
}

SkillTree::~SkillTree() {}

void SkillTree::loadFromJson(const std::string& filename) {
    // Open the JSON file and parse it
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
    }

    nlohmann::json skills = nlohmann::json::parse(file);

    for (const auto& skill : skills) {
        SkillNode node;
        node.id = skill["id"].get<std::string>(); //explicitely telling it to get a string for this id value!
        node.name = skill["name"].get<std::string>();
        node.description = skill["description"].get<std::string>();
        node.cost = skill["cost"].get<int>();
        node.magicType = static_cast<Magic>(skill["magic"].get<int>()); //Getting an integar, then casting it to the Magic enum
        node.upgradeType = static_cast<UpgradeKind>(skill["kind"].get<int>()); // Cast the integer to the UpgradeKind enum
        node.skillpathway = skill["skillpathway"].get<std::vector<std::string>>(); // Get the skill pathway as a vector of strings
        node.prerequisites = skill["prerequisites"].get<std::vector<std::string>>(); // Get the prerequisites as a vector of strings
        addSkillNode(node); // Add the skill node to the skill tree vector
    }

}

    void SkillTree::unlockSkill(const std::string& skillId)
    {
        if(canUnlockSkill(skillId)) //Checks if the skill can be unlocked, and if it can, unlocks it. If not, does nothing.
        {
            for (SkillNode& node : skillNodes)
            {
                if(node.id == skillId)
                {
                    node.isUnlocked = true;
                }
            }
        }

        else
        {
            std::cerr << "Cannot unlock skill - Prerequisites not met." << std::endl;
        }
    }

    bool SkillTree::canUnlockSkill(const std::string& skillId)
    {
        for (SkillNode& node : skillNodes){
            //Lets me iterate through ALL of the prerequisates within the vector, specifically by the string value of their id's
            if(node.id == skillId && !node.isUnlocked && std::all_of(node.prerequisites.begin(), node.prerequisites.end(), [this](const std::string& prereq) 
            //Lets me check if all the prerequisates for a given skill node are unlocked, by iterating through the skill tree vector and checking if the prerequisate id's match any of the skill node id's, and if they do, checking if they are unlocked
            {auto it = std::find_if(skillNodes.begin(), skillNodes.end(), [prereq](const SkillNode& node) { return node.id == prereq; }); 
            return it != skillNodes.end() && it->isUnlocked;})) //Will add tower Xp cost check here later...
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        return false; 
    }