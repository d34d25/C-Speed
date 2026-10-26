
#include "raylib.h"

#include "headers/scene.h"

int main(void)
{
    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "CSpeed");

    SetTargetFPS(60);

    RenderTexture2D canvas = LoadRenderTexture(NATIVE_SIZE, NATIVE_SIZE);

    InitAssets();

    Scene scene;

    InitScene(&scene);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        UpdateScene(&scene, dt);

        if(IsKeyPressed(KEY_R)) InitScene(&scene);

        TickSceneCouner(&scene);

        BeginTextureMode(canvas);
        
        DrawScene(&scene, dt);

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