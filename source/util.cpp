#include "util.h"

#include <cmath>

void Point::Rotate(float angle)
{
    const float sinA = sin(angle);
    const float cosA = cos(angle);

    const float nx = _x * cosA - _y * sinA;

    _y = _x * sinA + _y * cosA;
    _x = nx;
}
