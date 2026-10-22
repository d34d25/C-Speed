#ifndef ENEMY_H
#define ENEMY_H

#include "gameutils.h"

typedef enum EnemyType
{
    ENEMY_RED,
    ENEMY_GREEN,
    ENEMY_PINK,
    ENEMY_ORANGE,
    ASTEROID

} EnemyType;

typedef struct Enemy
{
    BulletPool bulletPool;

    SpriteData spriteData;

    float x;
    float y;

    float speed;

} Enemy;

void InitEnemy(Enemy* enemy, EnemyType type);

//enemy pool

typedef struct EnemyPool
{
    Enemy enemies[20];

    Enemy* activeEnemies[20];
    Enemy* inactiveEnemies[20];

}EnemyPool;

const int ENEMY_POOL_SIZE;

void InitEnemyPool(EnemyPool* enemypool, EnemyType type);

void UpdateEnemyPool(EnemyPool* enemypool, float dt);

void SpawnEnemy(EnemyPool* enemypool);

#endif

