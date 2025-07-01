#include "player.h"

#include "debug_ostream.h"
#include "key_logger.h"
using namespace DirectX;
#include "texture.h"
#include "sprite.h"

static XMFLOAT2 g_PlayerPosition{};
static XMFLOAT2 g_PlayerVelocity{};
static int g_PlayerTexId = -1;

void Player_Initialize(const XMFLOAT2 &position)
{
    g_PlayerPosition = position;
    g_PlayerVelocity = {0.0f, 0.0f};

    g_PlayerTexId = Texture_Load(L"assets/white.png");
}

void Player_Finalize()
{
}

void Player_Update(double elapsed_time)
{
    XMVECTOR position = XMLoadFloat2(&g_PlayerPosition);
    XMVECTOR velocity = XMLoadFloat2(&g_PlayerVelocity);

    XMVECTOR direction{};
    
    if (KeyLogger_IsPressed(KK_W))
    {
        direction += {0.0f, -1.0f};
    }
    if (KeyLogger_IsPressed(KK_S))
    {
        direction += {0.0f, 1.0f};
    }
    if (KeyLogger_IsPressed(KK_A))
    {
        direction += {-1.0f, 0.0f};
    }
    if (KeyLogger_IsPressed(KK_D))
    {
        direction += {1.0f, 0.0f};
    }

    direction = XMVector2Normalize(direction);

    velocity += direction * 2.0f;
    position += velocity;
    velocity *= 0.9f;
    
    XMStoreFloat2(&g_PlayerPosition, position);
    XMStoreFloat2(&g_PlayerVelocity, velocity);

    /************************************************************
    // physics simulation, used for any fps
    velocity += direction * 6000000.0f / 2500.0f * elapsed_time;
    position += velocity * elapsed_time;
    velocity += -velocity * 4.0f * elapsed_time;
    ************************************************************/
}

void Player_Draw()
{
    Sprite_Draw(g_PlayerTexId,
                g_PlayerPosition.x, g_PlayerPosition.y,
                256.0f, 256.0f
    );
}
