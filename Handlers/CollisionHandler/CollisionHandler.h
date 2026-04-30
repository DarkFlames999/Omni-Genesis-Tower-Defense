#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "../../Entities/Entities.h"
#include "../../Handlers/WaveHandler/WaveHandler.h"
#include "../../Handlers/EntityHandler/EntityHandler.h"

class CollisionHandler
{
public:
    CollisionHandler() = default;
    ~CollisionHandler() = default;

    // Check bullets hitting enemies
    void checkBulletEnemyCollision(std::vector<std::unique_ptr<Attack>>& attacks, EntityHandler& entities);

    // Check enemies hitting tower
    void checkEnemyTowerCollision(EntityHandler& entities, Tower& tower);

private:
    bool intersects(sf::FloatRect a, sf::FloatRect b) { return a.intersects(b); }
};

#endif