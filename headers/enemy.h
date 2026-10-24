#ifndef ENEMY_H
#define ENEMY_H

#include "gameutils.h"

typedef enum EnemyType
{
    ENEMY_RED,
    ENEMY_GREEN,
    ENEMY_PINK,
    ENEMY_ORANGE,
    ASTEROID,

    ENEMY_COUNT

} EnemyType;

typedef struct Enemy
{
    BulletPool bulletPool;

    SpriteData spriteData;

    Rectangle hitbox;

    EnemyType type;

    float size;

    float x;
    float y;

    float baseSpeedX;
    float baseSpeedY;

    float speedX;
    float speedY;

    bool rotated;

    bool alive;

    bool divedIntoPlayer;

} Enemy;

void InitEnemy(Enemy* enemy);

void UpdateEnemy(Enemy* enemy, Vector2 playerPos, float dt);

//enemy pool

typedef struct EnemyPool
{
    Enemy enemies[60];

    Enemy* activeEnemies[60];
    Enemy* inactiveEnemies[60];

    float fireTimer;
    float firerate;

}EnemyPool;

extern const int ENEMY_POOL_SIZE;

void InitEnemyPool(EnemyPool* enemypool, float firerate);

void UpdateEnemyPool(EnemyPool* enemypool, Vector2 playerPos, float dt);

void SpawnEnemy(EnemyPool* enemypool, EnemyType type);

void FireEnemy(EnemyPool* enemypool, float dt);

#endif

