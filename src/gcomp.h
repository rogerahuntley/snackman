#ifndef GCOMP_H
#define GCOMP_H

#include "ecpps/ecpps.h"

using namespace ecpps;

// holds some useful generic components
class PositionComponent;

typedef PositionComponent PositionC ;

struct PositionComponent: public Component {
    PositionComponent(int x = 0, int y = 0, int z = 0): x(x),y(y),z(z) {};
    float x;
    float y;
    float z;
};

struct ScaleComponent: public Component {
    ScaleComponent(int x = 0, int y = 0, int z = 0): x(x),y(y),z(z) {};
    float x;
    float y;
    float z;
};

struct RotationComponent: public Component {
    RotationComponent(int x = 0, int y = 0, int z = 0): x(x),y(y),z(z) {};
    float x;
    float y;
    float z;
};

#endif // GCOMP_H