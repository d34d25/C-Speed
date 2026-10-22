
#include "raylib.h"

#include "headers/player.h"

int main(void)
{
    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "CSpeed");

    SetTargetFPS(60);

    RenderTexture2D canvas = LoadRenderTexture(NATIVE_SIZE, NATIVE_SIZE);

    InitAssets();

    Player player;

    BulletPool bulletPool;

    InitBulletPool(&bulletPool, 0.2f, 3, 0.05f);

    InitPlayer(&player);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        UpdatePlayerInput(&player);

        UpdateBulletPool(&bulletPool, dt);

        //if(IsKeyPressed(KEY_Z)) SpawnBullet(&bulletPool, player.x, player.y, 128, 0, -1);

        FireBullet(&bulletPool, player.x, player.y, 128, 0, -1, IsKeyPressed(KEY_Z), IsKeyDown(KEY_Z), dt);

        UpdatePlayer(&player, dt);

        BeginTextureMode(canvas);

            ClearBackground(BLACK);

            for(int i = 0; i < BULLET_POOL_SIZE; i++)
            {
                Bullet* b = bulletPool.activeBullets[i];

                if(!b) continue;

                DrawSpriteData(&b->spriteData, b->x, b->y, dt);

                //DrawLine(player.x, player.y, b->x, b->y, RED);
            }

            DrawSpriteData(&player.spriteData, player.x, player.y, dt);

            //DrawCircle(player.x, player.y, 1, RED);

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