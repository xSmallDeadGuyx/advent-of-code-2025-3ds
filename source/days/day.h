#pragma once

class Day
{
public:
    virtual ~Day() = default;
    
    virtual bool Init();
    virtual bool Tick();
    virtual bool Draw();
    virtual void Cleanup();
};
