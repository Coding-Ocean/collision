#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"

static const int num = 11;
static float l = 1;
static VECTOR op[num] = {
    VECTOR(0,0,-0.5f),
    VECTOR(-0.433f,0,0.25f),
    VECTOR(0.433f,0,0.25f),
    VECTOR(-l, 0, -l),
    VECTOR(-l, 0, 0),
    VECTOR(-l, 0, l),
    VECTOR(0, 0, l),
    VECTOR(l, 0, l),
    VECTOR(l, 0, 0),
    VECTOR(l, 0, -l),
    VECTOR(0, 0, -l),
};
static VECTOR p[num];

void squareWithHole(VECTOR& t, VECTOR& r, const COLOR& c)
{
    gWorld.identity();
    gWorld.mulTranslate(t);
    gWorld.mulRotateYXZ(r);
    for (int i = 0; i < 11; i++) {
        p[i] = gWorld * op[i];
        p[i] = gView * p[i];
        p[i] = gProj * p[i];
    }
    //•`‰æ
    triangle3D(p[0], p[3], p[4], c, c, c);
    triangle3D(p[0], p[4], p[1], c, c, c);
    triangle3D(p[1], p[4], p[5], c, c, c);
    triangle3D(p[1], p[5], p[6], c, c, c);
    triangle3D(p[1], p[6], p[2], c, c, c);
    triangle3D(p[2], p[6], p[7], c, c, c);
    triangle3D(p[2], p[7], p[8], c, c, c);
    triangle3D(p[2], p[8], p[0], c, c, c);
    triangle3D(p[0], p[8], p[9], c, c, c);
    triangle3D(p[0], p[9], p[10], c, c, c);
    triangle3D(p[0], p[10], p[3], c, c, c);
}
