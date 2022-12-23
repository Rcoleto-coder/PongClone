#ifndef _VEC2_H_
#define _VEC2_H_
#include <iostream>

class Vec2
{
    friend std::ostream& operator << (std::ostream& os, const Vec2& obj);
         
public:
    float x;
    float y;
    // No args constructor
    Vec2()
	   : x(0.0f), y(0.0f)
    {}
    // Constructor
    Vec2(float x, float y)
	   : x(x), y(y)
    {}

    // Default destructor
    ~Vec2() = default;

    // Operators overload
    Vec2 operator+(Vec2 const& rhs)
    {
	   return Vec2(x + rhs.x, y + rhs.y);
    }

    Vec2& operator+=(Vec2 const& rhs)
    {
	   x += rhs.x;
	   y += rhs.y;

	   return *this;
    }

    Vec2 operator*(float rhs)
    {
	   return Vec2(x * rhs, y * rhs);
    }
    
};
#endif