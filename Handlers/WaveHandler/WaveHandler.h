/**
 * @file WaveHandler.h
 * @author Isaiah Watkins, Konner Knoll, and Keali Lake
 * @brief WaveHandler class declaration
 * @version 0.1
 * @date 2026-04-22
 */

#ifndef WAVEHANDLER_H
#define WAVEHANDLER_H

#include "../EntityHandler/EntityHandler.h"
#include "SFML/Graphics.hpp"
#include <string>
#include <cmath>
#include <queue>

class WaveHandler : public EntityHandler
{
public:
    WaveHandler();
    ~WaveHandler() = default;

    void Update(sf::RenderWindow& window, float deltaTime, Tower& tower);

    void SetWave(int wave);
    void StartNextWave(sf::RenderWindow& window);

    bool IsWaveComplete() const;

    bool IsWaveActive() const;

    int getCurrentWave() const { return mCurrentWave; }
    int GetTotalEnemiesThisWave() const { return mTotalEnemiesToSpawn; }
    int GetEnemiesRemainingToSpawn() const { return mEnemiesRemainingToSpawn; }

    void reset();

private:
    int ComputeEnemyCount(int wave) const;

    void BuildSpawnQueue(int wave);

    int mCurrentWave;
    int mTotalEnemiesToSpawn;
    int mEnemiesRemainingToSpawn;

    float mSpawnInterval;
    float mSpawnTimer;

    std::queue<std::string> mSpawnQueue;

    bool mAllSpawned;
};

#endif