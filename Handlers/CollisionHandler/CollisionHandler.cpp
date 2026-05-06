/**
 * @file CollisionHandler.cpp
 * @author Isaiah Watkins, Konner Knoll, and Keali Lake
 * @brief CollisionHandler.cpp class definitions, this allows our entities 
 * @version 0.1
 * @date 2026-04-27
 * 
 */

#include "CollisionHandler.h"

void CollisionHandler::checkBulletEnemyCollision(std::vector<std::unique_ptr<Attack>>& attacks, EntityHandler& entities)
{
        for(auto& bullet : attacks)
    {
        if(!bullet->isAlive()) continue;

        for(auto& enemy : entities.getEnemies())
        {
            if(!enemy) continue;

            Enemies* e = dynamic_cast<Enemies*>(enemy.get());
            if(!e) continue;
            if(e->isDead()) continue; // skip already dead enemies

            if(bullet->getCircleBounds().intersects(e->getHurtboxBounds()))
            {
                e->takeDamage(bullet->getDamage());
                bullet->setAlive(false); 
                std::cout << "Hit! Enemy health: " << e->getHealth() << std::endl;
                break;
            }
        }
    }
}

/**
 * @brief The enemy's hurtbox will be compared to the tower's hurtbox, and if they intersect, 
 * the tower will take damage and the enemy will be set to not alive so that it can be deleted 
 * in the next update cycle
 * 
 * @param entities 
 * @param tower 
 */
void CollisionHandler::checkEnemyTowerCollision(EntityHandler& entities, Tower& tower)
{
        for (auto& enemy : entities.getEnemies())
    {
        if (!enemy) continue;

        Enemies* enemies = dynamic_cast<Enemies*>(enemy.get());
        if (!enemies) continue;

        // Enemy hurtbox vs tower hurtbox
        if (enemies->getHurtboxBounds().intersects(tower.getHurtboxBounds()))
        {
            tower.takeDamage(enemies->getDamage());
            std::cout << "Tower health: " << tower.getHealth() << std::endl;
        }
    }
}