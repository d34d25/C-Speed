#include "../headers/enemy.h"

const int ENEMY_POOL_SIZE = 60;

const float MAX_ENEMY_INVUL_TIMER = 0.25f;

void InitEnemy(Enemy *enemy)
{
    enemy->x = 0.0f;
    enemy->y = -100.0f;

    enemy->baseSpeedX = 0.0f;

    enemy->baseSpeedY = 100.0f;

    enemy->alive = false;

    enemy->rotated = false;

    enemy->divedIntoPlayer = false;

    ResetEnemyFlags(enemy);

    enemy->health = 100.0f;

    enemy->type = ASTEROID;

    enemy->size = 6.0f;

    enemy->hitbox.x = enemy->x;
    enemy->hitbox.y = enemy->y;

    enemy->hitbox.width = enemy->size;
    enemy->hitbox.height = enemy->size;

    enemy->invulTimer = MAX_ENEMY_INVUL_TIMER;

    InitBulletPool(&enemy->bulletPool, 0.0f, 0.0f, 0.0f, 3.0f, 7);

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

void UpdateEnemy(Enemy* enemy, Vector2 playerPos, float dt)
{
    enemy->speedY = enemy->baseSpeedY;

    float x = enemy->x;
    float y = enemy->y;

    switch (enemy->type)
    {
    case ENEMY_RED:
    {
        if(enemy->y >= NATIVE_SIZE * 0.25f && !enemy->divedIntoPlayer)
        {
            if(x > playerPos.x)
            {
                enemy->speedX = -enemy->baseSpeedX;

                enemy->divedIntoPlayer = true;
            }
            else if(x < playerPos.x)
            {
                enemy->speedX = enemy->baseSpeedX;

                enemy->divedIntoPlayer = true;
            }
        }

        Vector2 dirToPlayer = Vector2Subtract(playerPos, (Vector2){enemy->x, enemy->y});

        dirToPlayer = Vector2Normalize(dirToPlayer);

        if(enemy->divedIntoPlayer && playerPos.y > y + 15 && enemy->alive)
        {
            FireBullet(
                &enemy->bulletPool,
                enemy->x,
                enemy->y,
                70.0f,
                dirToPlayer.x,
                dirToPlayer.y,
                false,
                true,
                false,
                dt
            );
        }
    }
    break;

    case ENEMY_GREEN:
    {
        bool hitLeftSide = enemy->x <= 0.0f;
        bool hitRightSide = enemy->x >= NATIVE_SIZE;

        if(hitLeftSide || hitRightSide)
        {
            float pushAmount = 2.5f;

            if(hitLeftSide)
            {
                enemy->x += pushAmount;
            }

            if(hitRightSide)
            {
                enemy->x -= pushAmount;
            }

            enemy->speedX *= -1.0f;
        }

        if(enemy->y > 0.0f)
        {
            FireBullet(
                &enemy->bulletPool,
                enemy->x,
                enemy->y,
                70.0f,
                0.0f,
                1.0f,
                false,
                true,
                false,
                dt
            );
        }
    }
    break;

    case ENEMY_PINK:
    {
        if(enemy->divedIntoPlayer) enemy->speedY = 0.0f;

        if(!enemy->divedIntoPlayer)
        {
            float threshold = 1.0f;

            bool topRange = enemy->y > playerPos.y - threshold;
            bool bottomRange = enemy->y < playerPos.y + threshold;

            if(topRange && bottomRange)
            {
                enemy->speedY = 0.0f;

                if(enemy->x > playerPos.x)
                {
                    enemy->speedX = -enemy->baseSpeedX;

                    enemy->spriteData.rotation = 90;
                }
                else if(enemy->x < playerPos.x)
                {
                    enemy->speedX = enemy->baseSpeedX;

                    enemy->spriteData.rotation = -90;
                }

                enemy->divedIntoPlayer = true;
            }
        }
    }
    break;

    case ENEMY_ORANGE:
    {
        if(enemy->y > -10 && enemy->y < NATIVE_SIZE + 10)
        {
            FireBullet(
                &enemy->bulletPool,
                enemy->x,
                enemy->y,
                70.0f,
                0.0f,
                1.0f,
                false,
                true,
                false,
                dt
            );
        }
    }
    break;
    
    default: break;
    }

    enemy->x += enemy->speedX * dt;
    enemy->y += enemy->speedY * dt;

    UpdateHitbox(&enemy->hitbox, enemy->x, enemy->y, enemy->size);

    enemy->invulTimer += dt;

    if(enemy->invulTimer >= MAX_ENEMY_INVUL_TIMER) enemy->invulTimer = MAX_ENEMY_INVUL_TIMER;
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

void UpdateEnemyPool(EnemyPool* enemypool, Vector2 playerPos, float dt)
{
    for(int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        //all enemies
        Enemy* eAll = &enemypool->enemies[i];
                
        UpdateBulletPool(&eAll->bulletPool, dt);

        //active enemies
        Enemy* e = enemypool->activeEnemies[i];

        if(!e) continue;

        UpdateEnemy(e, playerPos, dt);

        //bound checking

        bool dead = e->health <= 0.0f;

        if(IsPositionOutOfBounds(e->x, e->y, 10.0f) || dead)
        {
            e->alive = false;

            e->x = 1000;
            e->y = 1000;

            UpdateHitbox(&e->hitbox, e->x, e->y, e->size);
        }

        //swap from active to inactive
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

        UpdateHitbox(&e->hitbox, e->x, e->y, e->size);
        
        e->alive = true;

        e->rotated = false;

        e->divedIntoPlayer = false;

        ResetEnemyFlags(e);

        e->type = type;

        e->speedX = 0.0f;
        e->speedY = 0.0f;

        e->invulTimer = MAX_ENEMY_INVUL_TIMER;

        float firerate;
        float spread;

        int pelletCount;

        int frame;

        SpriteData* sprite = &e->spriteData;

        switch (e->type)
        {
        case ENEMY_RED:
        {
            spread = 0.04f;
            firerate = 0.8f;

            pelletCount = 3;

            e->baseSpeedY = 30.0f;
            e->baseSpeedX = 40.0f;

            frame = 1;

            e->health = 25.0f;
        }
        break;

        case ENEMY_GREEN:
        {
            firerate = 2.0f;
            spread = 0.80f;

            pelletCount = 3;
            
            int randValue = GetRandomValue(0, 100);

            int dirX;

            if(randValue < 50) dirX = -1;
            else dirX = 1; 

            e->baseSpeedY = 25.0f;
            e->baseSpeedX = 60.0f * dirX;

            e->speedX = e->baseSpeedX;
            
            frame = 2;

            e->health = 25.0f;
        }
        break;

        case ENEMY_PINK:
        {
            firerate = 0.0f;
            spread = 0.0f;

            pelletCount = 0;

            e->baseSpeedY = 40.0f;
            e->baseSpeedX = 70.0f;

            frame = 3;

            e->health = 25.0f;
        }
        break;

        case ENEMY_ORANGE:
        {
            firerate = 1.75f;
            spread = 0.0f;

            pelletCount = 3;

            e->baseSpeedY = 20.0f;
            e->baseSpeedX = 0.0f;

            frame = 4;

            e->health = 50.0f;
        }
        break;

        case ASTEROID:
        {
            firerate = 0.0f;
            spread = 0.0f;

            pelletCount = 0;

            e->baseSpeedY = 20.0f;
            e->baseSpeedX = 0.0f;

            frame = 5;

            e->health = 100.0f;
        }
        break;
        
        default:
        {
            firerate = 0.0f;
            spread = 0.0f;

            pelletCount = 0;

            e->baseSpeedY = 100.0f;
            e->baseSpeedX = 0.0f;

            frame = 5;

            e->health = 25.0f;
        } 
        break;
        }

        sprite->currentFrame = frame;

        sprite->frameStart = frame;
        sprite->frameEnd = frame;

        sprite->rotation = 0.0f;

        sprite->flipX = false;
        sprite->flipY = false;

        BulletPool* enemyBulletPool = &e->bulletPool;

        enemyBulletPool->firerate = firerate;
        enemyBulletPool->spread = spread;
        enemyBulletPool->pelletCount = pelletCount;

        enemyBulletPool->fireTimer = 0.0f;

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
