#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"
#include"mathUtil.h"

static const int num = 36;
static float rs = 0.013f;
static VECTOR opp[num] = {
    VECTOR(Cos(  0 * TO_RAD) * rs,Sin(  0 * TO_RAD) * rs,0),
    VECTOR(Cos( 45 * TO_RAD) * rs,Sin( 45 * TO_RAD) * rs,0),
    VECTOR(Cos( 90 * TO_RAD) * rs,Sin( 90 * TO_RAD) * rs,0),
    VECTOR(Cos(135 * TO_RAD) * rs,Sin(135 * TO_RAD) * rs,0),
    VECTOR(Cos(180 * TO_RAD) * rs,Sin(180 * TO_RAD) * rs,0),
    VECTOR(Cos(225 * TO_RAD) * rs,Sin(225 * TO_RAD) * rs,0),
    VECTOR(Cos(270 * TO_RAD) * rs,Sin(270 * TO_RAD) * rs,0),
    VECTOR(Cos(315 * TO_RAD) * rs,Sin(315 * TO_RAD) * rs,0),
};
void createCircle()
{
    float deg = 360.0f / num;
    for (int i = 0; i < num; i++)
    {
        //opp[i].set(0, Sin(deg * i * TO_RAD), Cos(deg * i * TO_RAD));
        opp[i].set(Cos(deg * i * TO_RAD), 0, Sin(deg * i * TO_RAD));
    }
}
void circle(const VECTOR& r, const COLOR& c)
{
    MATRIX world;
    world.identity();
    world.mulRotateYXZ(r);
    //‚»‚µ‚ÄÀ•W•ÏŠ·
    VECTOR p[num];
    for (int i = 0; i < num; i++) {
        p[i] = world * opp[i];
        p[i] = gView * p[i];
        p[i] = gProj * p[i];
    }
    //•`‰æ
    for (int i = 0; i < num-2; i++) {
        triangle3D(p[0], p[i + 1], p[i + 2], c, c, c);
    }
}
