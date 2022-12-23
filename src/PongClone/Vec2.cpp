#include "Vec2.h"

std::ostream& operator<<(std::ostream &os, const Vec2& obj)
{
    os << "x: " << obj.x << " y: " << obj.y;
    return os;
}


