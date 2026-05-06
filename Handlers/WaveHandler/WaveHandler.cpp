/**
 * @file WaveHandler.cpp
 * @author Isaiah Watkins, Konner Knoll, and Keali Lake
 * @brief WaveHandler class definitions - manages wave progression, timed spawning, and wave completion detection
 * @version 0.1
 * @date 2026-04-22
 */

#include "WaveHandler.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

// Seconds between each enemy spawn within a wave.
static constexpr float SPAWN_INTERVAL_SECONDS = 3.0f;

struct WaveTier
{
    int minWave;
    float juvenileRatio;
    float maturedRatio;
};

static constexpr WaveTier WAVE_TIERS[] = {
    { 1,  1.00f, 0.00f }, // waves  1–4:  all Juvenile
    { 5,  0.60f, 0.40f }, // waves  5–9:  60% Juvenile, 40% Matured
    { 10, 0.40f, 0.40f }, // waves 10–19: 40 / 40 / 20 split
    { 20, 0.20f, 0.50f }, // waves 20+:   20 / 50 / 30 split
    { 30, 0.10f, 0.50f }, // waves 30+:   10 / 50 / 40 split
    { 40, 0.00f, 0.50f }, // waves 40+:   0 / 50 / 50 split
    { 50, 0.00f, 0.40f }, // waves 50+:   0 / 40 / 60 split
    { 60, 0.00f, 0.30f }, // waves 60+:   0 / 30 / 70 split
    { 70, 0.00f, 0.20f }, // waves 70+:   0 / 20 / 80 split
    { 80, 0.00f, 0.10f }, // waves 80+:   0 / 10 / 90 split
    { 90, 0.00f, 0.00f }, // waves 90+:   ALL WARDENS - You are fucked
};

/**
 * @brief Selects the wave tier in WAVE_TIERS based on the current wave
 * 
 * @param wave
 * @return const WaveTier& 
 */
static const WaveTier& SelectTier(int wave)
{
    for (int i = static_cast<int>(std::size(WAVE_TIERS)) - 1; i >= 0; i--)
    {
        if (wave >= WAVE_TIERS[i].minWave)
        {
            return WAVE_TIERS[i];
        }
    }
    return WAVE_TIERS[0];
}

/**
 * @brief Construct a new Wave Handler:: Wave Handler object
 * 
 */
WaveHandler::WaveHandler() : EntityHandler()
    , mCurrentWave(0)
    , mTotalEnemiesToSpawn(0)
    , mEnemiesRemainingToSpawn(0)
    , mSpawnInterval(SPAWN_INTERVAL_SECONDS)
    , mSpawnTimer(0.f)
    , mAllSpawned(false)
{
}

/**
 * @brief Sets the current wave
 * 
 * @param wave 
 */
void WaveHandler::SetWave(int wave)
{
    mCurrentWave = wave;
}

/**
 * @brief Starts next wave and recomputes values
 * 
 * @param window 
 */
void WaveHandler::StartNextWave(sf::RenderWindow& window)
{
    mCurrentWave++;

    mTotalEnemiesToSpawn = ComputeEnemyCount(mCurrentWave);
    mEnemiesRemainingToSpawn = mTotalEnemiesToSpawn;
    mSpawnTimer = 0.f;
    mAllSpawned  = false;

    BuildSpawnQueue(mCurrentWave);
}

/**
 * @brief Update wave every tick
 * 
 * @param window 
 * @param deltaTime 
 */
void WaveHandler::Update(sf::RenderWindow& window, float deltaTime)
{
    if (!mAllSpawned)
    {
        mSpawnTimer += deltaTime;

        if (mSpawnTimer >= mSpawnInterval && !mSpawnQueue.empty())
        {
            mSpawnTimer -= mSpawnInterval;

            const std::string type = mSpawnQueue.front();
            mSpawnQueue.pop();

            SpawnEntity(type, window);
            mEnemiesRemainingToSpawn--;

            if (mSpawnQueue.empty())
            {
                mAllSpawned = true;
            }
        }
    }

    UpdateEntities(window, deltaTime);
}

/**
 * @brief Wave ends when every scheduled enemy has spawned AND all have been destroyed
 * 
 * @return true
 * @return false
 */
bool WaveHandler::IsWaveComplete() const
{
    return mAllSpawned && mEnemies.empty();
}

/**
 * @brief Checks if wave is active
 * 
 * @return true 
 * @return false 
 */
bool WaveHandler::IsWaveActive() const
{
    return !IsWaveComplete() && mCurrentWave > 0;
}

/**
 * @brief Computes number of enemies to appear per wave
 * 
 * @param wave 
 * @return int 
 */
int WaveHandler::ComputeEnemyCount(int wave) const
{
    // Formula: 10 + (n * (3.5 + 0.25 * n) / 10)
    const float n = static_cast<float>(wave);
    const float count  = 10.f + (n * (3.5f + 0.25f * n) / 10.f);
    return std::max(1, static_cast<int>(std::round(count)));
}

/**
 * @brief Build queue for enemies to spawn
 * 
 * @param wave 
 */
void WaveHandler::BuildSpawnQueue(int wave)
{
    while (!mSpawnQueue.empty()) {
        mSpawnQueue.pop();
    }

    const int total = mTotalEnemiesToSpawn;
    const WaveTier& tier = SelectTier(wave);

    const int juvenileCount = static_cast<int>(std::round(total * tier.juvenileRatio));
    const int maturedCount = static_cast<int>(std::round(total * tier.maturedRatio));
    const int wardenCount = total - juvenileCount - maturedCount;

    std::vector<std::string> enemies;
    enemies.reserve(total);

    for (int i = 0; i < juvenileCount; i++) {
        enemies.push_back("Juvenile");
    }
    for (int i = 0; i < maturedCount;  i++) {
        enemies.push_back("Matured");
    }
    for (int i = 0; i < wardenCount;   i++) {
        enemies.push_back("Warden");
    }

    // Fisher-Yates random shuffle
    for (int i = static_cast<int>(enemies.size()) - 1; i > 0; i--)
    {
        int j = std::rand() % (i + 1);
        std::swap(enemies[i], enemies[j]);
    }

    for (const auto& e : enemies){
        mSpawnQueue.push(e);
    }
}