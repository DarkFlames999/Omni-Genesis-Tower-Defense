#include "MagicSelection.h"
#include <iostream>

bool MagicIcon::contains(sf::Vector2f point) const {
    sf::FloatRect bounds(
        position.x - size.x / 2.f,
        position.y - size.y / 2.f,
        size.x,
        size.y
    );
    return bounds.contains(point);
}

void MagicSelection::init(const sf::RenderWindow& window, const sf::Font& font) {
    if (mInitialized) return;  // don't rebuild on re-entry
    mInitialized = true;

    // Load textures
    if (!mBraveryTex.loadFromFile("Sprites/BraverySkillTreeButton.png")) {
        std::cerr << "Failed to load BraverySkillTreeButton.png\n";
    }
    if (!mLockedTex.loadFromFile("Sprites/SkillTreeLocked.png")) {
        std::cerr << "Failed to load SkillTreeLocked.png\n";
    }

    // Title text
    mTitle.setFont(font);
    mTitle.setString("Choose Your Magic");
    mTitle.setCharacterSize(48);
    mTitle.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = mTitle.getLocalBounds();
    mTitle.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);
    mTitle.setPosition(window.getSize().x / 2.f, 80.f);

    struct MagicEntry { Magic m; std::string name; };
    std::vector<MagicEntry> entries = {
        { Magic::Determination, "Determination" },
        { Magic::Bravery,       "Bravery" },
        { Magic::Justice,       "Justice" },
        { Magic::Kindness,      "Kindness" },
        { Magic::Patience,      "Patience" },
        { Magic::Integrity,     "Integrity" },
        { Magic::Perseverance,  "Perseverance" },
        { Magic::Apathy,        "Apathy" },
        { Magic::Fear,          "Fear" },
        { Magic::Chaos,         "Chaos" },
        { Magic::Wrath,         "Wrath" },
        { Magic::Nihilistic,    "Nihilistic" },
        { Magic::Deceit,        "Deceit" },
        { Magic::Irresolution,  "Irresolution" },
    };

    // This is the grid layout
    const int columncount = 7;
    const int rowcount = 2;
    const float iconSize = 128.f;       
    const float horizontalSpacing = 40.f;
    const float verticalSpacing = 80.f;  
    const float labelOffset = 80.f;

    float gridWidth  = columncount * iconSize + (columncount - 1) * horizontalSpacing;
    float gridHeight = rowcount * iconSize + (rowcount - 1) * verticalSpacing;

    float startX = (window.getSize().x - gridWidth) / 2.f + iconSize / 2.f;
    float startY = (window.getSize().y - gridHeight) / 2.f + iconSize / 2.f;

    for (size_t i = 0; i < entries.size(); ++i) {
        int col = i % columncount;
        int row = i / columncount;

        MagicIcon icon;
        icon.magicType = entries[i].m;
        icon.displayName = entries[i].name;
        icon.size = sf::Vector2f(iconSize, iconSize);
        icon.position = sf::Vector2f(
            startX + col * (iconSize + horizontalSpacing),
            startY + row * (iconSize + verticalSpacing)
        );
        icon.isAccessible = (entries[i].m == Magic::Bravery);  // only Bravery for now

        // Configures sprite
        icon.sprite.setTexture(icon.isAccessible ? mBraveryTex : mLockedTex);
        icon.sprite.setOrigin(64.f, 64.f);  // center origin (texture is 128x128)
        icon.sprite.setPosition(icon.position);

        // Configures label
        icon.label.setFont(font);
        icon.label.setString(entries[i].name);
        icon.label.setCharacterSize(18);
        icon.label.setFillColor(icon.isAccessible ? sf::Color::White : sf::Color(150, 150, 150));
        sf::FloatRect labelBounds = icon.label.getLocalBounds();
        icon.label.setOrigin(labelBounds.width / 2.f, labelBounds.height / 2.f);
        icon.label.setPosition(icon.position.x, icon.position.y + labelOffset);

        mIcons.push_back(icon);
    }
}

bool MagicSelection::handleEvent(const sf::Event& e, const sf::RenderWindow& window, Magic& outClicked) {
    // Update hover state every mouse move
    if (e.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos = window.mapPixelToCoords(
            sf::Vector2i(e.mouseMove.x, e.mouseMove.y));
        for (auto& icon : mIcons) {
            icon.isHovered = icon.contains(mousePos);
        }
    }

    // Handling the click event in order to select our magic
    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords(
            sf::Vector2i(e.mouseButton.x, e.mouseButton.y));
        for (const auto& icon : mIcons) {
            if (icon.contains(mousePos) && icon.isAccessible) {
                outClicked = icon.magicType;
                return true;
            }
        }
    }
    return false;
}

void MagicSelection::draw(sf::RenderWindow& window) const {
    // Dimensions of background
    sf::RectangleShape dim(sf::Vector2f(window.getSize()));
    dim.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(dim);

    // Title
    window.draw(mTitle);

    // Each icon, with hover-scale on accessible ones
    for (const auto& icon : mIcons) {
        sf::Sprite spriteToDraw = icon.sprite;

        if (icon.isHovered && icon.isAccessible) {
            spriteToDraw.setScale(1.1f, 1.1f);  // slightly enlarges when hovering over it
        }

        window.draw(spriteToDraw);
        window.draw(icon.label);
    }
}