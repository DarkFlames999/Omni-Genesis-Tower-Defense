#ifndef MAGICSELECTION_H
#define MAGICSELECTION_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../SkillTree/SkillTree.h"   // for the Magic enum

struct MagicIcon {
    Magic magicType;
    std::string displayName;
    sf::Sprite sprite;
    sf::Text label;
    bool isAccessible;
    bool isHovered = false;
    sf::Vector2f position;
    sf::Vector2f size;

    bool contains(sf::Vector2f point) const;
};

class MagicSelection {
public:
    MagicSelection() = default;
    void init(const sf::RenderWindow& window, const sf::Font& font);
    bool handleEvent(const sf::Event& e, const sf::RenderWindow& window, Magic& outClicked);
    void draw(sf::RenderWindow& window) const;

private:
    std::vector<MagicIcon> mIcons;
    sf::Texture mBraveryTex;
    sf::Texture mLockedTex;
    sf::Text mTitle;
    bool mInitialized = false;
};

#endif