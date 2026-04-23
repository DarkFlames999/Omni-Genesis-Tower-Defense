/**
 * @file EntityHandler.cpp
 * @author Isaiah Watkins, Konner Knoll, and Keali Lake
 * @brief EntityHandler class definitions, which will be used to manage all the entities in the game, such as towers, enemies, and projectiles. 
 * This will be used to easily create new entities and manage them in the main game loop.
 * @version 0.1
 * @date 2026-04-15
 * 
 */
    #include <iostream>
    #include "SFML/Graphics.hpp"
    #include "EntityHandler.h"

    void EntityHandler::SpawnEntity(const std::string& type, sf::RenderWindow& window)
    {
         if(mEnemyTypeMapping.find(type) == mEnemyTypeMapping.end()) return;

         auto entity = mEnemyTypeMapping.find(type)->second();
         if(type == "Juvenile")
         {
            Juvenile* j = dynamic_cast<Juvenile*>(entity.get());
            if(j) 
            {
                j->createJuvenile(window, {0.f, 0.f}, {40.f, 130.f}, {30.f, 120.f});
            };
         }
         else if(type == "Matured")
         {
            Matured* m = dynamic_cast<Matured*>(entity.get());
            if(m) 
            {
                m->createMatured(window, {0.f, 0.f}, {40.f, 130.f}, {30.f, 120.f});
            };
         }
         else if(type == "Warden")
         {
            Warden* w = dynamic_cast<Warden*>(entity.get());
            if(w) 
            {
                w->createWarden(window, {0.f, 0.f}, {40.f, 130.f}, {30.f, 120.f});
            };
         }
               mEnemies.push_back(std::move(entity));

    }

    void EntityHandler::DrawEntities(sf::RenderWindow& window, sf::RenderStates states)
    {
        for(auto& enemies : mEnemies)
        {
            window.draw(*enemies, states);
        }

    }
    void EntityHandler::UpdateEntities(sf::RenderWindow& window)
    {
        for(auto& enemies : mEnemies)
        {
            enemies->update(window);
        }
    }