#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"

void square(const VECTOR* sqp, const COLOR& c)
{
    VECTOR p[4];
    for (int i = 0; i < 4; i++) {
        p[i] = gView * sqp[i];
        p[i] = gProj * p[i];
    }
    //•`‰æ
    triangle3D(p[0], p[1], p[2], c, c, c);
    triangle3D(p[2], p[1], p[3], c, c, c);
}
