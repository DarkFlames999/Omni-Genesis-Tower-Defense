/**
 * @file SkillTreeView.h
 * @brief Visual skill tree display with panning, zooming, and parallax background
 */
#ifndef SKILLTREEVIEW_H
#define SKILLTREEVIEW_H

#include <SFML/Graphics.hpp>
#include "SkillTree.h"

class SkillTreeView {
public:
    SkillTreeView() = default;
    void init(const sf::RenderWindow& window, const sf::Font& font, SkillTree& tree, Magic magic);
    std::string handleEvent(const sf::Event& e, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window, int playerXP) const;

private:
    SkillTree* mTree = nullptr;
    Magic mActiveMagic = Magic::Bravery;
    const sf::Font* mFont = nullptr;

    sf::Texture mBackgroundTex;
    sf::Texture mForegroundTex;
    sf::Sprite  mBackgroundSprite;
    sf::Sprite  mForegroundSprite;

    sf::View mTreeView;
    sf::Vector2f mPanOffset { 100.f, 100.f };


    bool mDragging = false;
    sf::Vector2i mLastMousePos;

    std::string mHoveredId;

    static constexpr float kNodeRadius = 30.f;
    static constexpr float kBackgroundParallax = 0.2f;
    static constexpr float kForegroundParallax = 0.5f;

    bool initialized = false;
    sf::Color colorForNode(const SkillNode& node) const;
    void drawConnections(sf::RenderWindow& window) const;
    void drawNodes(sf::RenderWindow& window) const;
    void drawTooltip(sf::RenderWindow& window) const;
    void drawHUD(sf::RenderWindow& window, int playerXP) const;
    
    std::vector<const SkillNode*> getActiveMagicSkills() const;
};

#endif