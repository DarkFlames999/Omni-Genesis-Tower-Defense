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

    // for (SkillNode& node : skillNodes) {
    //      switch(node.magicType)
    //      {
    //         case Magic::Determination:
    //             node.skillNodeShape.setColorButtonClick(sf::Color(255,0,0)); // Set the color of the skill node shape
    //             break;
    //          case Magic::Bravery:
    //              node.skillNodeShape.setColorButtonClick(sf::Color(255,165,0)); // Set the color of the skill node shape
    //              break;
    //         case Magic::Justice:
    //             node.skillNodeShape.setColorButtonClick(sf::Color(255,255,0)); // To Yellow
    //             break;
    //         case Magic::Kindness:
    //             node.skillNodeShape.setColorButtonClick(sf::Color(0,255,0)); // To Green
    //             break;
    //         case Magic::Patience:
    //             node.skillNodeShape.setColorButtonClick(sf::Color(0,255,255)); // To Cyan
    //             break;
    //         case Magic::Integrity:
    //             node.skillNodeShape.setColorButtonClick(sf::Color(0,0,255)); // To Blue
    //             break;
    //         case Magic::Perseverance:
    //             node.skillNodeShape.setColorButtonClick(sf::Color(255,0,255)); // To Magenta
    //             break;
    //         case Magic::Apathy:
    //             node.skillNodeShape.setColorButtonClick(sf::Color(100,0,0)); // To Dark Red
    //             break;
    //         case Magic::Fear:
    //             node.skillNodeShape.setColorButtonClick(sf::Color(255,140,0)); // To Dark Orange
    //             break;
    //         case Magic::Chaos:
    //             node.skillNodeShape.setColorButtonClick(sf::Color(100,75,0)); // To Dark Yellow
    //             break;
    //         case Magic::Wrath:
    //             node.skillNodeShape.setColorButtonClick(sf::Color(0,100,0)); // To Dark Green
    //             break;
    //         case Magic::Nihilistic:
    //             node.skillNodeShape.setColorButtonClick(sf::Color(0,100,100)); // To Dark Cyan
    //             break;
    //         case Magic::Deceit:
    //             node.skillNodeShape.setColorButtonClick(sf::Color(0,0,100)); // To Dark Blue
    //             break;
    //         case Magic::Irresolution:
    //             node.skillNodeShape.setColorButtonClick(sf::Color(100,0,100)); // To Dark Magenta
    //             break;
    //      }
    // }


}
Magic SkillTree::parseMagic(const std::string& magictype)
{
    std::string upperMagictype = magictype;
    std::transform(upperMagictype.begin(), upperMagictype.end(), upperMagictype.begin(), ::toupper);

    if (upperMagictype == "DETERMINATION")
    {
        return Magic::Determination;
    }
    else if (upperMagictype == "BRAVERY")
    {
        return Magic::Bravery;
    }
    else if (upperMagictype == "JUSTICE")
    {
        return Magic::Justice;
    }
    else if (upperMagictype == "KINDNESS")
    {
        return Magic::Kindness;
    }
    else if (upperMagictype == "PATIENCE")
    {
        return Magic::Patience;
    }
    else if (upperMagictype == "INTEGRITY")
    {
        return Magic::Integrity;
    }
    else if (upperMagictype == "PERSEVERANCE")
    {
        return Magic::Perseverance;
    }
    else if (upperMagictype == "APATHY")
    {
        return Magic::Apathy;
    }
    else if (upperMagictype == "FEAR")
    {
        return Magic::Fear;
    }
    else if (upperMagictype == "CHAOS")
    {
        return Magic::Chaos;
    }
    else if (upperMagictype == "WRATH")
    {
        return Magic::Wrath;
    }
    else if (upperMagictype == "NIHILISTIC")
    {
        return Magic::Nihilistic;
    }
    else if (upperMagictype == "DECEIT")
    {
        return Magic::Deceit;
    }   
    else if (upperMagictype == "IRRESOLUTION")
    {
        return Magic::Irresolution;
    }
    else
    {
        std::cout << "Invalid magic type: " << magictype << std::endl;
        exit(1);
    }
}

UpgradeKind SkillTree::parseUpgradeKind(const std::string& upgradekind)
{
    std::string upperUpgradekind = upgradekind;
    std::transform(upperUpgradekind.begin(), upperUpgradekind.end(), upperUpgradekind.begin(), ::toupper);

    if (upperUpgradekind == "OFFENSIVE")
    {
        return UpgradeKind::Offensive;
    }
    else if (upperUpgradekind == "DEFENSIVE")
    {
        return UpgradeKind::Defensive;
    }
    else if (upperUpgradekind == "UTILITY")
    {
        return UpgradeKind::Utility;
    }
    else
    {
        std::cout << "Invalid upgrade kind: " << upgradekind << std::endl;
        exit(1);
    }
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

    nlohmann::json data = nlohmann::json::parse(file);

    for (const auto& skill : data["skills"]) {
        SkillNode node;
        node.id = skill["id"].get<std::string>(); //explicitely telling it to get a string for this id value!
        node.name = skill["name"].get<std::string>();
        node.description = skill["description"].get<std::string>();
        node.cost = skill["cost"].get<int>();
        node.magicType = parseMagic(skill["magic"].get<std::string>()); //Getting a string, then casting it to the Magic enum
        node.upgradeType = parseUpgradeKind(skill["kind"].get<std::string>()); // Cast the string to the UpgradeKind enum
        node.skillpathway = skill["skillpathway"].get<std::vector<std::string>>(); // Get the skill pathway as a vector of strings
        node.prerequisites = skill["prerequisites"].get<std::vector<std::string>>(); // Get the prerequisites as a vector of strings
        node.position = sf::Vector2f(skill["position"][0].get<float>(), skill["position"][1].get<float>()); // Get the position as a vector of floats
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
        auto it = std::find_if(skillNodes.begin(), skillNodes.end(), [&skillId] (const SkillNode& node) { return node.id == skillId; });
        //Searches within the SkillNodes vector for the skill with the ID that matches the skillId parameter

            //If already unlocked or skill isn't found within the vector
            if (it == skillNodes.end() || it->isUnlocked) 
            {
                return false;
            }
            
            //Returns all of the prerequisites for the skill, checking if they are unlocked. This is only true is all prerequiresites are unlocked, otherwise it returns false. If there are no prerequisites, this will return true.
            return std::all_of(it->prerequisites.begin(), it->prerequisites.end(),[this](const std::string& prereq) 
            {auto prerequisiteIt = std::find_if(skillNodes.begin(), skillNodes.end(), [&](const SkillNode& prerequitenode) { return prerequitenode.id == prereq; }); return (prerequisiteIt != skillNodes.end() && prerequisiteIt->isUnlocked);});

        return false; 
    }

    SkillNode* SkillTree::findSkill(const std::string& id) {
    for (auto& node : skillNodes) {
        if (node.id == id) return &node;
    }
    return nullptr;
}

void SkillTree::reset()
{
    for (auto& node : skillNodes) 
    {
        node.isUnlocked = false;
    }
}