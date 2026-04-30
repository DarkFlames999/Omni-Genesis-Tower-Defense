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

            // Hitbox vs Hurtbox — bullet's hitbox hits enemy's hurtbox
            if(bullet->getHitboxBounds().intersects(e->getHurtboxBounds()))
            {
                bullet->setAlive(false);
                e->takeDamage(bullet->getDamage());
                std::cout << "Hit! Enemy health: " << e->getHealth() << std::endl;
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
    for(auto& enemy : entities.getEnemies())
    {
        if(!enemy) continue;

        Enemies* e = dynamic_cast<Enemies*>(enemy.get());
        if(!e) continue;

        // Enemy hurtbox vs tower hurtbox
        if(e->getHitboxBounds().intersects(tower.getHurtboxBounds()))
        {
            tower.takeDamage(e->getDamage());
            std::cout << "Tower health: " << tower.getHealth() << std::endl;
        }
    }
}