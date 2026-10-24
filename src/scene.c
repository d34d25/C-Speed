#include "../headers/scene.h"

void InitScene(Scene *scene)
{
    InitPlayer(&scene->player);

    InitEnemyPool(&scene->enemyPool, 1.5f);

    InitBulletPool(&scene->backgroundStars, 1.15f, 1, 0.0f, 15.0f, 11);
}

void UpdateScene(Scene *scene, float dt)
{
    scene->enemyPool.firerate -= 0.005f * dt;

    if(scene->enemyPool.firerate <= 0.0f) scene->enemyPool.firerate = 0.0f;

    UpdateBulletPool(&scene->backgroundStars, dt);

    FireBullet(
        &scene->backgroundStars,
        (float)GetRandomValue(0, NATIVE_SIZE),
        (float)GetRandomValue(-NATIVE_SIZE * 0.5f, 0),
        20.0f, 
        0.0f,
        1.0f,
        false,
        true,
        false,
        dt
    );

    UpdatePlayerInput(&scene->player);

    UpdatePlayer(&scene->player, dt);

    UpdateEnemyPool(&scene->enemyPool, (Vector2){scene->player.x, scene->player.y}, dt);

    FireEnemy(&scene->enemyPool, dt);

    //collisions

    CollideScreenEdges(&scene->player.hitbox, &scene->player.x, &scene->player.y);

    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        Bullet* playerBullet =  scene->player.bulletPool.activeBullets[i];

        if(!playerBullet) continue;

        for(int e = 0; e < ENEMY_POOL_SIZE; e++)
        {
            Enemy* activeEnemy = scene->enemyPool.activeEnemies[e];

            if(!activeEnemy) continue;

            if(CheckCollisionCircleRec(
                (Vector2){playerBullet->x, playerBullet->y},
                playerBullet->radius,
                activeEnemy->hitbox
            ))
            {
                playerBullet->alive = false;
            }
        }
    }
}

void DrawScene(Scene *scene, float dt)
{
    ClearBackground(BLACK);

    for(int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        Enemy* eAll = &scene->enemyPool.enemies[i];

        DrawSpriteData(&eAll->spriteData, eAll->x, eAll->y, dt);

        for(int a = 0; a < BULLET_POOL_SIZE; a++)
        {
                Bullet* b = eAll->bulletPool.activeBullets[a];

                if(!b) continue;

                DrawSpriteData(&b->spriteData, b->x, b->y, dt);
        }
    }

    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        Bullet* star = scene->backgroundStars.activeBullets[i];

        if(star)
        {
            Color starColor = WHITE;

            starColor.a = 100; //GetRandomValue(100, 200);

            DrawRectangle(star->x, star->y, 1, 1, starColor);
        }

        Bullet* b = scene->player.bulletPool.activeBullets[i];

        if(!b) continue;

        DrawSpriteData(&b->spriteData, b->x, b->y, dt);
    }

    DrawSpriteData(&scene->player.spriteData, scene->player.x, scene->player.y, dt);
}
