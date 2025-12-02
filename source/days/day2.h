#pragma once

#include "day.h"

class Day2 : public Day
{
public:
    virtual bool Init() override final;
    virtual bool Tick() override final;
    virtual void Cleanup() override final;
};
