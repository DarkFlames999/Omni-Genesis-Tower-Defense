/**
 * @file SkillTreeView.cpp
 */
#include "SkillTreeView.h"
#include <algorithm>
#include <iostream>
#include <sstream>

void SkillTreeView::init(const sf::RenderWindow& window, const sf::Font& font, 
                         SkillTree& tree, Magic magic) {
    mTree = &tree;
    mActiveMagic = magic;
    mFont = &font;

    mPanOffset = sf::Vector2f(0.f, 0.f);
    mDragging = false;
    mHoveredId.clear();

    mTreeView.setSize(static_cast<float>(window.getSize().x),
                      static_cast<float>(window.getSize().y));
    mTreeView.setCenter(0.f, 0.f);

    if (initialized) return;
    initialized = true;

    if (!mBackgroundTex.loadFromFile("Sprites/SpaceBackground.png")) {
        std::cerr << "Failed to load SpaceBackground.png\n";
    }
    if (!mForegroundTex.loadFromFile("Sprites/SpaceForeground.png")) {
        std::cerr << "Failed to load SpaceForeground.png\n";
    }
    mBackgroundSprite.setTexture(mBackgroundTex);
    mForegroundSprite.setTexture(mForegroundTex);


    auto scaleSprite = [&](sf::Sprite& sprite, const sf::Texture& tex) {
        sf::Vector2u winSize = window.getSize();
        sf::Vector2u texSize = tex.getSize();
        if (texSize.x == 0 || texSize.y == 0) return;

        float padding = 1.5f;
        float scaleX = (winSize.x * padding) / texSize.x;
        float scaleY = (winSize.y * padding) / texSize.y;
        float scale = std::max(scaleX, scaleY);
        
        sprite.setScale(scale, scale);
        sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
    };

    scaleSprite(mBackgroundSprite, mBackgroundTex);
    scaleSprite(mForegroundSprite, mForegroundTex);
}

std::string SkillTreeView::handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
    
    if (e.type == sf::Event::MouseButtonPressed && 
        e.mouseButton.button == sf::Mouse::Right) {
        mDragging = true;
        mLastMousePos = sf::Mouse::getPosition(window);
        return "";
    }
    if (e.type == sf::Event::MouseButtonReleased && 
        e.mouseButton.button == sf::Mouse::Right) {
        mDragging = false;
        return "";
    }
    if (e.type == sf::Event::MouseMoved && mDragging) {
        sf::Vector2i current(e.mouseMove.x, e.mouseMove.y);
        sf::Vector2i delta = mLastMousePos - current;

        mTreeView.move(static_cast<float>(delta.x), static_cast<float>(delta.y));
        mPanOffset += sf::Vector2f(static_cast<float>(delta.x), 
                                    static_cast<float>(delta.y));
        mLastMousePos = current;
        return "";
    }

    if (e.type == sf::Event::MouseWheelScrolled) {
        float zoomFactor = (e.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
        mTreeView.zoom(zoomFactor);
        return "";
    }

    if (e.type == sf::Event::MouseMoved && !mDragging) {
        sf::Vector2f worldPos = window.mapPixelToCoords(
            sf::Vector2i(e.mouseMove.x, e.mouseMove.y), mTreeView);
        mHoveredId.clear();
        for (const auto* node : getActiveMagicSkills()) {
            sf::Vector2f diff = worldPos - node->position;
            float distSq = diff.x * diff.x + diff.y * diff.y;
            if (distSq <= kNodeRadius * kNodeRadius) {
                mHoveredId = node->id;
                break;
            }
        }
    }

    if (e.type == sf::Event::MouseButtonPressed && 
        e.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f worldPos = window.mapPixelToCoords(
            sf::Vector2i(e.mouseButton.x, e.mouseButton.y), mTreeView);
        for (const auto* node : getActiveMagicSkills()) {
            sf::Vector2f diff = worldPos - node->position;
            float distSq = diff.x * diff.x + diff.y * diff.y;
            if (distSq <= kNodeRadius * kNodeRadius) {
                return node->id;
            }
        }
    }

    return "";
}

std::vector<const SkillNode*> SkillTreeView::getActiveMagicSkills() const {
    std::vector<const SkillNode*> result;
    if (!mTree) return result;
    for (const auto& node : mTree->getSkillNodes()) {
        if (node.magicType == mActiveMagic) {
            result.push_back(&node);
        }
    }
    return result;
}

sf::Color SkillTreeView::colorForNode(const SkillNode& node) const {
    if (node.isUnlocked) {
        return sf::Color(255, 215, 0);
    }
    
    bool available = true;
    for (const auto& prereqId : node.prerequisites) {
        bool found = false;
        for (const auto& other : mTree->getSkillNodes()) {
            if (other.id == prereqId && other.isUnlocked) {
                found = true;
                break;
            }
        }
        if (!found) {
            available = false;
            break;
        }
    }
    
    if (available) {
        return sf::Color(100, 200, 255);
    }
    return sf::Color(80, 80, 80);
}

void SkillTreeView::drawConnections(sf::RenderWindow& window) const {
    sf::VertexArray lines(sf::Lines);
    for (const auto* node : getActiveMagicSkills()) {
        for (const auto& prereqId : node->prerequisites) {
            // find the prereq's position
            for (const auto& other : mTree->getSkillNodes()) {
                if (other.id == prereqId && other.magicType == mActiveMagic) {
                    sf::Color lineColor = node->isUnlocked && other.isUnlocked
                        ? sf::Color(255, 215, 0, 200)
                        : sf::Color(120, 120, 120, 150);
                    lines.append(sf::Vertex(node->position, lineColor));
                    lines.append(sf::Vertex(other.position, lineColor));
                    break;
                }
            }
        }
    }
    window.draw(lines);
}

void SkillTreeView::drawNodes(sf::RenderWindow& window) const {
    for (const auto* node : getActiveMagicSkills()) {
        sf::CircleShape circle(kNodeRadius);
        circle.setOrigin(kNodeRadius, kNodeRadius);
        circle.setPosition(node->position);
        circle.setFillColor(colorForNode(*node));
        circle.setOutlineColor(sf::Color::White);
        circle.setOutlineThickness(node->id == mHoveredId ? 12.f : 2.f);
        window.draw(circle);

        // Label below
        if (mFont) {
            sf::Text label(node->name, *mFont, 16);
            label.setFillColor(sf::Color::White);
            sf::FloatRect b = label.getLocalBounds();
            label.setOrigin(b.width / 2.f, 0.f);
            label.setPosition(node->position.x, node->position.y + kNodeRadius + 5.f);
            window.draw(label);
        }
    }
}

void SkillTreeView::drawTooltip(sf::RenderWindow& window) const {
    if (mHoveredId.empty() || !mFont || !mTree) return;
    
    const SkillNode* node = nullptr;
    for (const auto& nodes : mTree->getSkillNodes()) {
        if (nodes.id == mHoveredId) {
            node = &nodes;
            break;
        }
    }
    if (!node) return;

    std::stringstream ss;
    ss << node->name << "\n";
    ss << "\n" << node->description << "\n";
    ss << "\nCost: " << node->cost << " XP";
    if (!node->prerequisites.empty()) {
        ss << "\n\nRequires:";
        for (const auto& prereqId : node->prerequisites) {
            for (const auto& other : mTree->getSkillNodes()) {
                if (other.id == prereqId) {
                    ss << "\n  - " << other.name;
                    break;
                }
            }
        }
    }

    window.setView(window.getDefaultView());
    
    sf::Text tipText(ss.str(), *mFont, 16);
    tipText.setFillColor(sf::Color::White);
    
    sf::Vector2i mousePix = sf::Mouse::getPosition(window);
    sf::FloatRect bounds = tipText.getLocalBounds();
    float MouseOffset = 10.f; //Offset from mouse and padding inside background
    float backgroundW = bounds.width + MouseOffset * 2;
    float backgroundH = bounds.height + MouseOffset * 2;
    
    float x = mousePix.x + 20.f;
    float y = mousePix.y + 20.f;
    if (x + backgroundW > window.getSize().x) x = window.getSize().x - backgroundW - 10.f;
    if (y + backgroundH > window.getSize().y) y = window.getSize().y - backgroundH - 10.f;
    
    sf::RectangleShape background(sf::Vector2f(backgroundW, backgroundH));
    background.setPosition(x, y);
    background.setFillColor(sf::Color(0, 0, 0, 220));
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(1.f);
    window.draw(background);
    
    tipText.setPosition(x + MouseOffset, y + MouseOffset);
    window.draw(tipText);
    
    window.setView(mTreeView);
}

void SkillTreeView::drawHUD(sf::RenderWindow& window, int playerXP) const {
    window.setView(window.getDefaultView());
    
    if (!mFont) return;
    
    std::stringstream ss;
    ss << "XP: " << playerXP;
    
    sf::Text xpText(ss.str(), *mFont, 28);
    xpText.setFillColor(sf::Color::White);
    xpText.setPosition(20.f, 20.f);
    
    sf::FloatRect b = xpText.getLocalBounds();
    sf::RectangleShape bg(sf::Vector2f(b.width + 20.f, b.height + 20.f));
    bg.setPosition(15.f, 15.f);
    bg.setFillColor(sf::Color(0, 0, 0, 180));
    bg.setOutlineColor(sf::Color::White);
    bg.setOutlineThickness(1.f);
    window.draw(bg);
    window.draw(xpText);
}

void SkillTreeView::draw(sf::RenderWindow& window, int playerXP) const {
    window.setView(window.getDefaultView());
    sf::Vector2f windowCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);

    sf::Sprite background = mBackgroundSprite;
    background.setPosition(windowCenter.x - mPanOffset.x * kBackgroundParallax, 
                          windowCenter.y - mPanOffset.y * kBackgroundParallax);
    window.draw(background);

    sf::Sprite foreground = mForegroundSprite;
    foreground.setPosition(windowCenter.x - mPanOffset.x * kForegroundParallax, 
                          windowCenter.y - mPanOffset.y * kForegroundParallax);
    window.draw(foreground);
    window.setView(mTreeView);
    drawConnections(window);
    drawNodes(window);
    drawHUD(window, playerXP);
    drawTooltip(window);

    window.setView(window.getDefaultView());
}