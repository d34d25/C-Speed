#include "player.h"

float playerBaseFirerate = 0.4f;

float playerBaseSpread = 0.0f;

float playerBaseDamage = 25.0f;

int playerBasePelletCount = 1;

int maxPelletCount = 8;

float maxFireRate = 0.15f;

float maxSpread = 0.8f;

void InitPlayer(Player* player)
{
    player->lives = 3;

    player->hit = false;
    player->wasHit = false;

    player->x = NATIVE_SIZE * 0.5f;
    player->y = NATIVE_SIZE * 0.8f;

    player->speed = 60.0f;

    ResetPlayerInput(player);

    player->dead = false;

    player->size = 6.0f;

    player->hitbox.x = player->x;
    player->hitbox.y = player->y;

    player->hitbox.width = player->size;
    player->hitbox.height = player->size;

    player->damage = 25.0f;

    player->powerUpMaxTime = 10.0f;

    player->fireRatePowerUpTimer = player->powerUpMaxTime;

    player->maxInvulTimer = 1.5f;

    player->invulTimer = player->maxInvulTimer;

    InitBulletPool(&player->bulletPool, playerBaseFirerate, playerBasePelletCount, playerBaseSpread, 1.5f, 6);

    SpriteData* sprite = &player->spriteData;

    sprite->animationSpeed = 0.0f;
    
    sprite->flipX = false;
    sprite->flipY = true;

    sprite->currentFrame = 0;

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

    player->shooting = IsKeyDown(KEY_Z);
    
    player->shootingPress = IsKeyPressed(KEY_Z); 
}

void UpdatePlayer(Player *player, float dt)
{
    //bulletpool

    UpdateBulletPool(&player->bulletPool, dt);

    FireBullet(
        &player->bulletPool,
        player->x, player->y,
        128.0f, 0, -1,
        player->shootingPress, player->shooting, true,
        dt
    );
    
    //player speed and position

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

    UpdateHitbox(&player->hitbox, player->x, player->y, player->size);

    player->bulletPool.spread = Clamp(player->bulletPool.spread, 0.0f, maxSpread);

    player->bulletPool.pelletCount = Clamp((float)player->bulletPool.pelletCount, 0.0f, (float)maxPelletCount);

    player->bulletPool.firerate = Clamp(player->bulletPool.firerate, maxFireRate, playerBaseFirerate);

    player->damage = Clamp(player->damage, 0.0f, 100.0f);

    player->fireRatePowerUpTimer += dt;

    player->pelletCountPowerUpTimer += dt;

    player->damagePowerUpTimer += dt;

    if(player->fireRatePowerUpTimer >= player->powerUpMaxTime)
    {
        player->fireRatePowerUpTimer = player->powerUpMaxTime;

        player->bulletPool.firerate = playerBaseFirerate;
    }

    if(player->pelletCountPowerUpTimer >= player->powerUpMaxTime)
    {
        player->bulletPool.spread = playerBaseSpread;;

        player->bulletPool.pelletCount = playerBasePelletCount;
    }

    if(player->damagePowerUpTimer >= player->powerUpMaxTime)
    {
        player->damage = playerBaseDamage;
    }

    player->invulTimer += dt;

    if(player->invulTimer >= player->maxInvulTimer) player->invulTimer = player->maxInvulTimer;

    if(!IsPlayerAlive(player) && !player->dead)
    {
        player->spriteData.animationSpeed = 12.0f;

        player->spriteData.frameStart = 11;

        player->spriteData.frameEnd = 18;

        if(player->spriteData.currentFrame >= 17.0f) player->dead = true;
    }

    if(player->dead)
    {
        player->spriteData.frameStart = 18;

        player->spriteData.frameEnd = 18;
    }

}

void UpdatePlayerFlags(Player *player)
{
    player->wasHit = player->hit;
}

void ResetPlayerInput(Player *player)
{
    player->movingRight = false;
    player->movingLeft = false;

    player->movingDown = false;
    player->movingUp = false;

    player->shootingPress = false;
    player->shooting = false;
}

bool IsPlayerAlive(Player *player)
{
    if(!player) return false;

    return player->lives > 0;
}

void ResetPlayerFlags(Player *player)
{
    player->hit = false;
    player->wasHit = false;
}
