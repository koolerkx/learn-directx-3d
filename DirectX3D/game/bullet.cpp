#include "bullet.h"

using namespace DirectX;

class Bullet
{
private:
    XMFLOAT3 position_ = {};
    XMFLOAT3 velocity_ = {};
    double accumulated_time_ = 0.0;
    static constexpr double LIFE_TIME = 3.0;

public:
    Bullet(const XMFLOAT3& position, const XMFLOAT3& velocity) :
        position_(position), velocity_(velocity)
    {}

    void Update(double elapsed_time)
    {
        accumulated_time_ += elapsed_time;
        XMVECTOR position = XMLoadFloat3(&position_);
        XMVECTOR velocity = XMLoadFloat3(&velocity_);
        position += velocity * static_cast<float>(accumulated_time_);
        XMStoreFloat3(&position_, position);
    }

    const XMFLOAT3& GetPosition() const { return position_; }

    XMFLOAT3 GetFront() const
    {
        XMFLOAT3 front;
        XMStoreFloat3(&front, XMVector3Normalize(XMLoadFloat3(&velocity_)));
        return front;
    }

    bool IsDestroy() const { return accumulated_time_ >= LIFE_TIME; }
};

static constexpr int MAX_BULLETS = 1024;
static Bullet* g_Bullets[MAX_BULLETS] = {};
static int g_BulletCount = 0;

void Bullet_Initialize() {}

void Bullet_Finalize()
{
    for (int i = 0; i < g_BulletCount; i++)
    {
        delete g_Bullets[i];
        g_Bullets[i] = nullptr;
    }
    g_BulletCount = 0;
}

void Bullet_Update(double elapsed_time)
{
    for (int i = 0; i < g_BulletCount; i++)
    {
        if (g_Bullets[i]->IsDestroy())
        {
            Bullet_Destroy(i);
        }
    }

    for (int i = 0; i < g_BulletCount; i++)
    {
        g_Bullets[i]->Update(elapsed_time);
    }
}

void Bullet_Draw() {}

void Bullet_Create(
    const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& velocity
)
{
    if (g_BulletCount + 1 >= MAX_BULLETS)
    {
        return;
    }
    g_Bullets[g_BulletCount] = new Bullet(position, velocity);
    g_BulletCount++;
}

void Bullet_Destroy(int index)
{
    delete g_Bullets[index];
    g_Bullets[index] = g_Bullets[g_BulletCount - 1];
    g_BulletCount--;
}

int Bullet_GetObjectsCount() { return g_BulletCount; }

AABB Bullet_GetAABB(int index) { return {}; }
