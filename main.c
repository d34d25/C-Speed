
#include "raylib.h"

#include "headers/player.h"

#include "headers/enemy.h"

int main(void)
{
    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "CSpeed");

    SetTargetFPS(60);

    RenderTexture2D canvas = LoadRenderTexture(NATIVE_SIZE, NATIVE_SIZE);

    InitAssets();

    Player player;

    BulletPool bulletPool;

    EnemyPool enemyPool;

    InitBulletPool(&bulletPool, 0.15f, 3, 0.05f, 1.5f);

    InitEnemyPool(&enemyPool, 1.5f);

    InitPlayer(&player);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        enemyPool.firerate -= 0.005f * dt;

        if(enemyPool.firerate <= 0.0f) enemyPool.firerate = 0.0f;

        UpdatePlayerInput(&player);

        UpdateBulletPool(&bulletPool, dt);

        FireBullet(&bulletPool, player.x, player.y, 128.0f, 0, -1, IsKeyPressed(KEY_Z), IsKeyDown(KEY_Z), dt);

        UpdatePlayer(&player, dt);

        UpdateEnemyPool(&enemyPool, (Vector2){player.x, player.y}, dt);

        FireEnemy(&enemyPool, dt);

        BeginTextureMode(canvas);

            ClearBackground(BLACK);

            for(int i = 0; i < ENEMY_POOL_SIZE; i++)
            {
                Enemy* eAll = &enemyPool.enemies[i];

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
                Bullet* b = bulletPool.activeBullets[i];

                if(!b) continue;

                DrawSpriteData(&b->spriteData, b->x, b->y, dt);
            }

            DrawSpriteData(&player.spriteData, player.x, player.y, dt);

        EndTextureMode();

        BeginDrawing();

            ClearBackground(GRAY);

            DrawTexturePro(
                canvas.texture,
                (Rectangle){0.0f, 0.0f, (float)canvas.texture.width, (float)-canvas.texture.height},
                (Rectangle){halfScreen, halfScreen, SCREEN_SIZE, SCREEN_SIZE},
                (Vector2){halfScreen, halfScreen},
                0.0f,
                WHITE
            );

            DrawFPS(10,10);

        EndDrawing();
    }

    UnloadAssets();

    UnloadRenderTexture(canvas);
    
    CloseWindow();

    return 0;
}