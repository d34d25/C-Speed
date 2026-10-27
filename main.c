#include "raylib.h"

#include "scene.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

Scene scene;

RenderTexture2D canvas;

void RunGame()
{
    float dt = GetFrameTime();

    UpdateScene(&scene, dt);

    if(IsKeyPressed(KEY_R)) InitScene(&scene);

    TickSceneCounter(&scene);

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

int main(void)
{
    InitWindow(SCREEN_SIZE, SCREEN_SIZE, "CSpeed");

    SetTargetFPS(60);

    canvas = LoadRenderTexture(NATIVE_SIZE, NATIVE_SIZE);

    InitAssets();    

    InitScene(&scene);

    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(RunGame, 0, 1);
    #else

    while (!WindowShouldClose())
    {
        RunGame();
    }

    #endif

    UnloadScene(&scene);

    UnloadAssets();

    UnloadRenderTexture(canvas);
    
    CloseWindow();

    return 0;
}