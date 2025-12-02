#pragma once

#include "day.h"

#include <vector>
#include <cstdint>

class Day1 : public Day
{
public:
    virtual bool Init() override final;
    virtual bool Tick() override final;
    virtual bool Draw() override final;
    virtual void Cleanup() override final;

private:
    void HandleSpin();

    static const float _nonTurboLerpTime;
    static const uint32_t _turboSpinsPerTick;

    // Parsed input
    std::vector<int> _nums;

    // Solver runtime data
    bool _turboMode = false;
    float _nonTurboLerpAlpha = 0.f;
    uint16_t _index = 0;
    uint8_t _curNum = 50;
    uint32_t _zeroCount = 0;
    uint32_t _zeroClicks = 0;

    // Visualization-only data
    std::vector<uint32_t> _visFromNum;
    std::vector<uint32_t> _visToNum;
    std::vector<int> _visClicks;
    float _visLastAnimNum = 50.f;
    float _visClickAlpha = 0.f;
};
