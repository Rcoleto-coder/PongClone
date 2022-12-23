#ifndef _CONTACT_H_
#define _CONTACT_H_
#include "Ball.h"
#include "Paddle.h"

enum class CollisionType
{
    None,
    Top,
    Middle,
    Bottom,
    Left,
    Right
};

class Contact
{
public:
    
    CollisionType type;
    float penetration;

    Contact() {
        type= CollisionType::None;
        penetration = 0.0f;
    }

    Contact(CollisionType type, float penetration)
        :type(type), penetration(penetration) {}

    ~Contact() = default;

};



#endif // !_CONTACT_H_

