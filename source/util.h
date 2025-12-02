#pragma once

#include <citro2d.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

#define PI 3.141592653589793

static constexpr float KINDA_SMALL_NUMBER = 0.0001f;
static constexpr float FRAME_TIME = 1.f / 60.f;

static constexpr u32 COL_WHITE = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
static constexpr u32 COL_GREEN = C2D_Color32(0x00, 0xFF, 0x00, 0xFF);
static constexpr u32 COL_RED   = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);
static constexpr u32 COL_BLUE  = C2D_Color32(0x00, 0x00, 0xFF, 0xFF);


template <typename T>
int Sign(T val)
{
    return (T(0) < val) - (val < T(0));
}

template <typename T>
T WrapNum(T num, T min, T max)
{
    const T diff = max - min;
    while (num < min)
    {
        num += diff;
    }

    while (num >= max)
    {
        num -= diff;
    }

    return num;
}

struct Point
{
public:
    Point() = default;
    Point(float inX, float inY) : _x(inX), _y(inY) {}

    float _x = 0.f;
    float _y = 0.f;

    void Rotate(float angle);
};
