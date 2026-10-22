#include "../headers/enemy.h"

void InitEnemy(Enemy *enemy, EnemyType type)
{
    enemy->x = 0.0f;
    enemy->y = -100.0f;

    float firerate;
    float spread;

    int pelletCount;

    switch (type)
    {
    case ENEMY_RED:
    {
        spread = 0.02f;
        firerate = 2.25f;

        pelletCount = 1;
    }
    break;

    case ENEMY_GREEN:
    {
        firerate = 2.0f;
        spread = 0.01f;

        pelletCount = 2;
    }
    break;

    case ENEMY_PINK:
    {
        firerate = 0.0f;
        spread = 0.0f;

        pelletCount = 0;
    }
    break;

    case ENEMY_ORANGE:
    {
        firerate = 1.75f;
        spread = 0.0f;

        pelletCount = 3;
    }
    break;

    case ASTEROID:
    {
        firerate = 0.0f;
        spread = 0.0f;

        pelletCount = 0;
    }
    break;
    
    default:
        break;
    }

    InitBulletPool(&enemy->bulletPool, firerate, pelletCount, spread);
}

void InitEnemyPool(EnemyPool *enemypool, EnemyType type)
{

}

void UpdateEnemyPool(EnemyPool *enemypool, float dt)
{

}

void SpawnEnemy(EnemyPool *enemypool)
{

}
