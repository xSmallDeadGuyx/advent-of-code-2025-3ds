#include "day1.h"

#include "../util.h"

#include <3ds.h>
#include <citro2d.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdio>

const float Day1::_nonTurboLerpTime = 0.5f;
const uint32_t Day1::_turboSpinsPerTick = 15;

bool Day1::Init()
{
    FILE* f = fopen("romfs:/inputs/day1.txt", "r");
    if (f == nullptr)
    {
        printf("\x1b[6;1HFailed to open day1.txt");
        return false;
    }

    char line[100];
    while (fgets(line, sizeof(line), f))
    {
        line[strcspn(line, "\r\n")] = 0; // kill any newlines _and_ carriage returns

        line[0] = line[0] == 'L' ? '-' : '0';

        _nums.emplace_back(atoi(line));
    }

    fclose(f);

    _index = 0;
    _curNum = 50;
    _zeroCount = 0;
    _nonTurboLerpAlpha = 0.f;
    return true;
}

void Day1::HandleSpin()
{
    const int nextNum = _nums[_index];

    int clicks = abs(nextNum) / 100;

    const int nextNumMod = nextNum % 100;

    if (_curNum != 0 && (_curNum + nextNumMod <= 0 || _curNum + nextNumMod >= 100))
    {
        clicks += 1;
    }

    _visFromNum.emplace_back(_curNum);

    _curNum = (_curNum + nextNumMod + 100) % 100;

    _visToNum.emplace_back(_curNum);

    if (_curNum == 0)
    {
        _zeroCount += 1;
    }

    _zeroClicks += clicks;
    _visClicks.emplace_back(clicks);

    _index++;
}

bool Day1::Tick()
{
    u32 kDown = hidKeysDown();
    if (kDown & KEY_A)
    {
        _turboMode = !_turboMode;
        _nonTurboLerpAlpha = 0.f;
    }

    if (_index < _nums.size())
    {
        if (_turboMode)
        {
            for (uint8_t i = 0; i < _turboSpinsPerTick && _index < _nums.size(); ++i)
            {
                HandleSpin();
            }
        }
        else
        {
            _nonTurboLerpAlpha += FRAME_TIME / _nonTurboLerpTime;
            if (_nonTurboLerpAlpha >= 1.f - KINDA_SMALL_NUMBER)
            {
                _nonTurboLerpAlpha = 0.f;

                HandleSpin();
            }
        }
    }

    printf("\x1b[1;1HPress A to toggle turbo. Index = %d", _index);
    
    constexpr uint32_t numsToDraw = 20;

    const uint32_t startIdx = _index < numsToDraw ? 0 : _index - numsToDraw;
    for (uint32_t i = startIdx; i < _index; ++i)
    {
        printf("\x1b[%lu;1H%lu + %c%d = %lu (%d clicks)\x1b[K", 3 + i - startIdx, _visFromNum[i], _nums[i] < 0 ? 'L' : 'R', abs(_nums[i]), _visToNum[i], _visClicks[i]);
    }

    printf("\x1b[%lu;1HZero count = %lu. Zero clicks = %lu", 3 + numsToDraw + 1, _zeroCount, _zeroClicks);

    return true;
}

bool Day1::Draw()
{
    C2D_DrawCircleSolid(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, SCREEN_HEIGHT / 2 - 40, COL_BLUE);

    float triNum = static_cast<float>(_curNum);
    u32 triCol = _curNum == 0 ? COL_GREEN : COL_RED;

    if (!_turboMode && _index < _nums.size())
    {
        const float curMove = static_cast<float>(_nums[_index]);
        triNum = WrapNum(triNum + (_nonTurboLerpAlpha * curMove), -50.f, 50.f);

        if (Sign(triNum) != Sign(_visLastAnimNum) && Sign(triNum) == Sign(curMove))
        {
            // Crossed the zero threshold this frame
            _visClickAlpha = 1.f;
        }
        else if (_visClickAlpha > 0.f)
        {
            _visClickAlpha -= FRAME_TIME / 0.2f;
            if (_visClickAlpha < 0.f)
            {
                _visClickAlpha = 0.f;
            }
        }

        triCol = C2D_Color32f(1.f - _visClickAlpha, _visClickAlpha, 0.f, 1.f);
    }

    _visLastAnimNum = triNum;

    Point pointA(0.f, -SCREEN_HEIGHT / 2 + 25);
    Point pointB(10.f, -SCREEN_HEIGHT / 2 + 60);
    Point pointC(-10.f, -SCREEN_HEIGHT / 2 + 60);

    const float angle = triNum * PI / 50.f;

    pointA.Rotate(angle);
    pointB.Rotate(angle);
    pointC.Rotate(angle);

    constexpr float centerX = SCREEN_WIDTH / 2;
    constexpr float centerY = SCREEN_HEIGHT / 2;

    C2D_DrawTriangle(centerX + pointA._x, centerY + pointA._y, triCol, centerX + pointB._x, centerY + pointB._y, triCol, centerX + pointC._x, centerY + pointC._y, triCol, 1);

    return true;
}

void Day1::Cleanup()
{
    _nums.clear();

    _visFromNum.clear();
    _visToNum.clear();
    _visClicks.clear();
}
