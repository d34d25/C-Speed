#include "scene.h"

void InitScene(Scene *scene)
{
    InitPlayer(&scene->player);

    InitEnemyPool(&scene->enemyPool, 1.5f);

    InitBulletPool(&scene->backgroundStars, 0.5f, 1, 0.0f, 15.0f, 11);

    InitPowerUpPool(&scene->powerUps);

    for(int i = 0; i < 10; i++)
    {
        SpawnBullet(
            &scene->backgroundStars,
            (float)GetRandomValue(0, NATIVE_SIZE),
            (float)GetRandomValue(-NATIVE_SIZE, NATIVE_SIZE),
            60.0f,
            0.0f,
            1.0f,
            false
        );
    }

    scene->timer = 0.0f;

    scene->frameCounter = 0;

    scene->gameover = false;

    scene->inTitle = true;

    Image titleImage = LoadImage("assets/title.png");

    scene->titleTexture = LoadTextureFromImage(titleImage);

    UnloadImage(titleImage);
}

void UpdateScene(Scene *scene, float dt)
{
    UpdateBulletPool(&scene->backgroundStars, dt);

    FireBullet(
        &scene->backgroundStars,
        (float)GetRandomValue(0, NATIVE_SIZE),
        (float)GetRandomValue(-NATIVE_SIZE * 0.5f, 0),
        60.0f, 
        0.0f,
        1.0f,
        false,
        true,
        false,
        dt
    );

    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_Z)) scene->inTitle = false;

    if(scene->inTitle) return;

    if(IsPlayerAlive(&scene->player)) scene->timer += dt;

    ResetPlayerFlags(&scene->player);

    for(int e = 0; e < ENEMY_POOL_SIZE; e++)
    {
        Enemy* activeEnemy = scene->enemyPool.activeEnemies[e];

        if(activeEnemy) ResetEnemyFlags(scene->enemyPool.activeEnemies[e]);
    }

    scene->enemyPool.firerate -= 0.005f * dt;

    float maxEnemySpawnRate = 0.001f;

    if(scene->enemyPool.firerate <= maxEnemySpawnRate) scene->enemyPool.firerate = maxEnemySpawnRate;    

    if(IsPlayerAlive(&scene->player)) UpdatePlayerInput(&scene->player);
    else ResetPlayerInput(&scene->player);

    UpdatePlayer(&scene->player, dt);

    UpdateEnemyPool(&scene->enemyPool, (Vector2){scene->player.x, scene->player.y}, dt);

    FireEnemy(&scene->enemyPool, dt);

    UpdatePowerUPPool(&scene->powerUps, dt);

    //collisions

    CollideScreenEdges(&scene->player.hitbox, &scene->player.x, &scene->player.y);

    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        Bullet* playerBullet = scene->player.bulletPool.activeBullets[i];

        PowerUP* currentPowerUp = scene->powerUps.activePowerUps[i];

        if(currentPowerUp && IsPlayerAlive(&scene->player))
        {
            if(CheckCollisionCircleRec(
                (Vector2){currentPowerUp->x, currentPowerUp->y},
                currentPowerUp->radius,
                scene->player.hitbox
            ))
            {
                currentPowerUp->alive = false;

                switch (currentPowerUp->type)
                {
                case FIRERATE_UP:
                {
                    scene->player.bulletPool.firerate -= 0.025f;

                    scene->player.fireRatePowerUpTimer = 0.0f;
                }
                break;

                case PELLET_COUNT_UP:
                {
                    scene->player.bulletPool.pelletCount++;

                    scene->player.bulletPool.spread += 0.025f;

                    scene->player.pelletCountPowerUpTimer = 0.0f;
                }
                break;

                case DAMAGE_UP:
                {
                    scene->player.damage += 25.0f;

                    scene->player.damagePowerUpTimer = 0.0f;
                }
                break;
                
                default: break;
                }
            }
        }

        for(int e = 0; e < ENEMY_POOL_SIZE; e++)
        {
            Enemy* activeEnemy = scene->enemyPool.activeEnemies[e];
            
            if(activeEnemy)
            {
                if(CheckCollisionRecs(scene->player.hitbox, activeEnemy->hitbox) && IsEnemyAlive(activeEnemy))
                {
                    scene->player.hit = true;
                }
            }

            if(playerBullet && activeEnemy)
            {
                if(CheckCollisionCircleRec(
                (Vector2){playerBullet->x, playerBullet->y},
                playerBullet->radius,
                activeEnemy->hitbox
                ) && IsEnemyAlive(activeEnemy))
                {
                    playerBullet->alive = false;

                    activeEnemy->hit = true;
                }
            }

            Bullet* enemyBullet = scene->enemyPool.enemies[e].bulletPool.activeBullets[i];

            if(enemyBullet)
            {
                if(CheckCollisionCircleRec(
                (Vector2){enemyBullet->x, enemyBullet->y},
                enemyBullet->radius,
                scene->player.hitbox
                ) && IsPlayerAlive(&scene->player))
                {
                    enemyBullet->alive = false;

                    scene->player.hit = true;
                }
            }

            if(!activeEnemy) continue;

            if(!activeEnemy->wasHit && activeEnemy->hit && activeEnemy->invulTimer >= MAX_ENEMY_INVUL_TIMER)
            {
                activeEnemy->health -= scene->player.damage;

                activeEnemy->invulTimer = 0.0f;

                if(activeEnemy->health <= 0.0f)
                {
                    int powerUpSpawnChance = GetRandomValue(0, 100);

                    if(powerUpSpawnChance <= 20)
                    {
                        int powerUpType = GetRandomValue(0, POWER_UP_TYPE_COUNT - 1);

                        SpawnPowerUp(&scene->powerUps, activeEnemy->x, activeEnemy->y, powerUpType);
                    }
                }
            }

            UpdateEnemyFlags(activeEnemy);
        }
    }

    if(!scene->player.wasHit && scene->player.hit && scene->player.invulTimer >= scene->player.maxInvulTimer)
    {
        scene->player.lives--;

        scene->player.invulTimer = 0.0f;

        if(!IsPlayerAlive(&scene->player)) scene->player.lives = 0;
    }

    UpdatePlayerFlags(&scene->player);

    if(scene->player.dead) scene->gameover = true;
}

void DrawScene(Scene *scene, float dt)
{
    ClearBackground(BLACK);

    DrawRectangleLines(0, 0, NATIVE_SIZE, NATIVE_SIZE, PICO_WHITE);

    if(scene->inTitle)
    {
        for(int i = 0; i < BULLET_POOL_SIZE; i++)
        {
            Bullet* star = scene->backgroundStars.activeBullets[i];

            if(!star) continue;
            
            Color starColor = PICO_WHITE;

            starColor.a = 100;

            DrawRectangle(star->x, star->y, 1, 1, starColor);
            
        }

        DrawTexturePro(
            scene->titleTexture,
            (Rectangle){0, 0, NATIVE_SIZE, NATIVE_SIZE},
            (Rectangle){0, 0, NATIVE_SIZE, NATIVE_SIZE},
            (Vector2){0.0f, 0.0f},
            0.0f,
            WHITE
        );

        if(scene->frameCounter > 60)
        {
            DrawText("PRESS ENTER / Z", 17, 90, 1, PICO_WHITE);

            DrawText("TO START", 35, 105, 1, PICO_WHITE);
        }

        return;
    }

    for(int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        Enemy* eAll = &scene->enemyPool.enemies[i];

        Enemy* e = scene->enemyPool.activeEnemies[i];

        if(e)
        {
            if(e->invulTimer < MAX_ENEMY_INVUL_TIMER)
            {
                if(scene->frameCounter % 2 == 0) DrawSpriteData(&e->spriteData, e->x, e->y, dt);
            }
            else
            {
                DrawSpriteData(&e->spriteData, e->x, e->y, dt);
            }
        }

        for(int a = 0; a < BULLET_POOL_SIZE; a++)
        {
            Bullet* b = eAll->bulletPool.activeBullets[a];

            if(b) DrawSpriteData(&b->spriteData, b->x, b->y, dt);
        }
    }

    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        Bullet* star = scene->backgroundStars.activeBullets[i];

        if(star)
        {
            Color starColor = PICO_WHITE;

            starColor.a = 100; //GetRandomValue(100, 200);

            DrawRectangle(star->x, star->y, 1, 1, starColor);
        }

        Bullet* b = scene->player.bulletPool.activeBullets[i];

        if(b) DrawSpriteData(&b->spriteData, b->x, b->y, dt);

        PowerUP* p = scene->powerUps.activePowerUps[i];

        if(p) DrawSpriteData(&p->spriteData, p->x, p->y, dt);
    }

    if(scene->player.invulTimer < scene->player.maxInvulTimer && IsPlayerAlive(&scene->player))
    {
        if(scene->frameCounter % 4 == 0 ) DrawSpriteData(&scene->player.spriteData, scene->player.x, scene->player.y, dt);
    }
    else
    {
        DrawSpriteData(&scene->player.spriteData, scene->player.x, scene->player.y, dt);
    }

    DrawText(TextFormat("Time: %i", (int)scene->timer), NATIVE_SIZE - 40, 1, 1, PICO_WHITE);

    for(int i = 1; i <= scene->player.lives; i++)
    {
        DrawSprite(0, 8 * i, 8 * 1, 8, 8, 0, false, true);
    }

    if(scene->gameover)
    {
        DrawText("Game Over", 40, 50, 1, PICO_WHITE); 

        if(scene->frameCounter > 60)
        {
            DrawText("Prees R to restart", 20, 66, 1, PICO_WHITE);
        }
    }
}

void TickSceneCounter(Scene *scene)
{
    scene->frameCounter++;

    if(scene->frameCounter >= 120) scene->frameCounter = 0;
}

void UnloadScene(Scene *scene)
{
    UnloadTexture(scene->titleTexture);
}
