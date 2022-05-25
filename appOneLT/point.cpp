#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"
#include"mathUtil.h"

float rs = 0.015f;
VECTOR opp[] = {
    VECTOR(Cos(0) * rs,Sin(0) * rs,0),
    VECTOR(Cos(45 * TO_RAD) * rs,Sin(45 * TO_RAD) * rs,0),
    VECTOR(Cos(90 * TO_RAD) * rs,Sin(90 * TO_RAD) * rs,0),
    VECTOR(Cos(135 * TO_RAD) * rs,Sin(135 * TO_RAD) * rs,0),
    VECTOR(Cos(180 * TO_RAD) * rs,Sin(180 * TO_RAD) * rs,0),
    VECTOR(Cos(225 * TO_RAD) * rs,Sin(225 * TO_RAD) * rs,0),
    VECTOR(Cos(270 * TO_RAD) * rs,Sin(270 * TO_RAD) * rs,0),
    VECTOR(Cos(315 * TO_RAD) * rs,Sin(315 * TO_RAD) * rs,0),
};

void point(const VECTOR& t, const COLOR& c)
{
    
    gWorld.identity();
    gWorld.mulTranslate(t);
    //ÇªÇµÇƒç¿ïWïœä∑
    VECTOR p[8];
    for (int i = 0; i < 8; i++) {
        p[i] = gWorld * opp[i];
        p[i] = gView * p[i];
        p[i] = gProj * p[i];
    }
    //ï`âÊ
    for (int i = 0; i < 6; i++) {
        triangle3D(p[0], p[i + 1], p[i + 2], c, c, c);
    }
}
