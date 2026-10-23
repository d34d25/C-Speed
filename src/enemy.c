#include "../headers/enemy.h"
#include "enemy.h"

const int ENEMY_POOL_SIZE = 60;

void InitEnemy(Enemy *enemy)
{
    enemy->x = 0.0f;
    enemy->y = -100.0f;

    enemy->alive = false;

    enemy->rotated = false;    

    InitBulletPool(&enemy->bulletPool, 0.0f, 0.0f, 0.0f);

    SpriteData* sprite = &enemy->spriteData;

    sprite->animationSpeed = 0.0f;
    
    sprite->flipX = false;
    sprite->flipY = false;

    sprite->currentFrame = 5;

    sprite->frameStart = 5;
    sprite->frameEnd = 5;

    sprite->width = 8;
    sprite->height = 8;

    sprite->rotation = 0.0f;
}

void UpdateEnemy(Enemy *enemy, float dt)
{
    enemy->y += enemy->speedY * dt;
}

void InitEnemyPool(EnemyPool *enemypool, float firerate)
{
    for(int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        InitEnemy(&enemypool->enemies[i]);

        enemypool->inactiveEnemies[i] = &enemypool->enemies[i];

        enemypool->activeEnemies[i] = NULL;
    }

    enemypool->firerate = firerate;
    enemypool->fireTimer = 0.0f;
}

void UpdateEnemyPool(EnemyPool *enemypool, float dt)
{
    //active enemies
    for(int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        Enemy* e = enemypool->activeEnemies[i];

        if(e == NULL) continue;

        UpdateEnemy(e, dt);

        //bound checking
        if(e->y - 10 > NATIVE_SIZE || e->x <= -10 || e->x + 10 >= NATIVE_SIZE)
        {
            e->alive = false;
        }

        if(!e->alive)
        {
            enemypool->inactiveEnemies[i] = e;

            enemypool->activeEnemies[i] = NULL;
        }
    }
}

void SpawnEnemy(EnemyPool* enemypool, EnemyType type)
{
    Enemy* e;
    
    int enemyIndex = 0;

    //inactive enemies
    for(int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        Enemy* currentEnemy = enemypool->inactiveEnemies[i];

        if(currentEnemy == NULL) continue;
        else
        {
            e = currentEnemy;
            enemyIndex = i;
        }
    }

    if(e != NULL && enemyIndex > 0 && enemyIndex < ENEMY_POOL_SIZE)
    {
        e->x = (float)GetRandomValue(0, NATIVE_SIZE);
        e->y = (float)GetRandomValue(-NATIVE_SIZE, 0);

        e->alive = true;

        e->rotated = false;

        float firerate;
        float spread;

        int pelletCount;

        int frame;

        SpriteData* sprite = &e->spriteData;

        switch (type)
        {
            case ENEMY_RED:
        {
            spread = 0.02f;
            firerate = 2.25f;

            pelletCount = 1;

            e->speedY = 30.0f;
            e->speedX = 40.0f;

            frame = 1;
        }
        break;

        case ENEMY_GREEN:
        {
            firerate = 2.0f;
            spread = 0.01f;

            pelletCount = 2;

            e->speedY = 25.0f;
            e->speedX = 45.0f;

            frame = 2;
        }
        break;

        case ENEMY_PINK:
        {
            firerate = 0.0f;
            spread = 0.0f;

            pelletCount = 0;

            e->speedY = 40.0f;
            e->speedX = 40.0f;

            frame = 3;
        }
        break;

        case ENEMY_ORANGE:
        {
            firerate = 1.75f;
            spread = 0.0f;

            pelletCount = 3;

            e->speedY = 20.0f;
            e->speedX = 0.0f;

            frame = 4;
        }
        break;

        case ASTEROID:
        {
            firerate = 0.0f;
            spread = 0.0f;

            pelletCount = 0;

            e->speedY = 20.0f;
            e->speedX = 0.0f;

            frame = 5;
        }
        break;
        
        default:
        {
            firerate = 0.0f;
            spread = 0.0f;

            pelletCount = 0;

            e->speedY = 100.0f;
            e->speedX = 0.0f;

            frame = 5;
        } 
        break;
        }

        sprite->currentFrame = frame;

        sprite->frameStart = frame;
        sprite->frameEnd = frame;

        sprite->rotation = 0.0f;

        sprite->flipX = false;
        sprite->flipY = false;

        BulletPool* enmeyBulletPool = &e->bulletPool;

        enmeyBulletPool->firerate = firerate;
        enmeyBulletPool->spread = spread;
        enmeyBulletPool->pelletCount = pelletCount;

        enmeyBulletPool->fireTimer = 0.0f;

        enemypool->activeEnemies[enemyIndex] = e;

        enemypool->inactiveEnemies[enemyIndex] = NULL;
    }
}

void FireEnemy(EnemyPool *enemypool, float dt)
{
    enemypool->fireTimer += dt;

    bool canSpawn = false;

    if(enemypool->fireTimer > enemypool->firerate)
    {
        enemypool->fireTimer = 0.0f;

        canSpawn = true;
    }
    else
    {
        canSpawn = false;
    }

    if(canSpawn)
    {
        EnemyType spawnType = GetRandomValue(0, ENEMY_COUNT - 1);

        SpawnEnemy(enemypool, spawnType);
    }
}
