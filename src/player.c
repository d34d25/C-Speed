#include "../headers/player.h"

void InitPlayer(Player* player)
{
    player->x = NATIVE_SIZE * 0.5f;
    player->y = NATIVE_SIZE * 0.8f;

    player->speed = 60.0f;

    player->movingRight = false;
    player->movingLeft = false;

    player->movingDown = false;
    player->movingUp = false;

    player->shooting = false;

    SpriteData* sprite = &player->spriteData;

    sprite->animationSpeed = 0.0f;
    sprite->currentFrame = 0;
    sprite->flipX = false;
    sprite->flipY = true;

    sprite->frameStart = 0;
    sprite->frameEnd = 0;

    sprite->width = 8;
    sprite->height = 8;

    sprite->rotation = 0.0f;
}

void UpdatePlayerInput(Player *player)
{
    player->movingRight = IsKeyDown(KEY_RIGHT);
    player->movingLeft = IsKeyDown(KEY_LEFT);

    player->movingUp = IsKeyDown(KEY_UP);
    player->movingDown = IsKeyDown(KEY_DOWN);
}

void UpdatePlayer(Player *player, float dt)
{
    Vector2 playerSpeed = {0.0f, 0.0f};

    float speed = player->speed * dt;

    if(player->movingRight)
    {
        playerSpeed.x = speed;
    }
    else if(player->movingLeft)
    {
        playerSpeed.x = -speed;
    }

    if(player->movingUp)
    {
        playerSpeed.y = -speed;
    }
    else if(player->movingDown)
    {
        playerSpeed.y = speed;
    }

    Vector2 finalSpeed = Vector2Normalize(playerSpeed);

    player->x += finalSpeed.x;
    player->y += finalSpeed.y;
}