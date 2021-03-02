#ifndef GCOMP_H
#define GCOMP_H

#include "ecpps/ecpps.h"

using namespace ecpps;

// holds some useful generic components
class PositionComponent;

typedef PositionComponent PositionC ;

struct PositionComponent: public Component {
    PositionComponent(int x = 0, int y = 0): x(x),y(y) {};
    float x;
    float y;
};

struct ScaleComponent: public Component {
    ScaleComponent(int x = 1, int y = 1): x(x),y(y) {};
    float x;
    float y;
};

struct RotationComponent: public Component {
    RotationComponent(int x = 0): x(x) {};
    float x;
};

struct CollisionComponent: public Component {

};

struct CollisionBoxComponent: public CollisionComponent {

};

struct CollisionSet: public Component {
    
};

#endif // GCOMP_H