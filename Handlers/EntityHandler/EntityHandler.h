/**
 * @file EntityHandler.h
 * @author Isaiah Watkins, Konner Knoll, and Keali Lake
 * @brief 
 * @version 0.1
 * @date 2026-04-27
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef ENTITYHANDLER_H
#define ENTITYHANDLER_H

#include "../../Entities/Entities.h"
#include "SFML/Graphics.hpp"
#include <vector>
#include <memory>
#include <map>
#include <string>
#include <functional>

class EntityHandler
{
    public:
        using EntitySpawning = std::function<std::unique_ptr<Entity>()>;

        EntityHandler()
        {
            mEnemyTypeMapping["Juvenile"] = []() {return std::make_unique<Juvenile>();}; 
            mEnemyTypeMapping["Matured"] = []() {return std::make_unique<Matured>();};
            mEnemyTypeMapping["Warden"] = []() {return std::make_unique<Warden>();};  
        }
        ~EntityHandler(){};

        void SpawnEntity(const std::string& type, sf::RenderWindow& window);
        
        void DrawEntities(sf::RenderWindow& window, sf::RenderStates states); //For each entity 
        void UpdateEntities(sf::RenderWindow& window, float deltaTime);
        std::vector<std::unique_ptr<Entity>>& getEnemies() { return mEnemies; }

    protected:
        std::vector<std::unique_ptr<Entity>> mEnemies;
        std::map<std::string, EntitySpawning> mEnemyTypeMapping;
        
};


#endif