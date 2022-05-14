//--------------------------
// 三角形の穴が開いた正方形
//--------------------------
#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"

float l = 1;
VECTOR osqp[11] = {
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

void squareWithHole(VECTOR& t, VECTOR& r, const COLOR& c)
{
    MATRIX world;
    world.identity();
    world.mulTranslate(t);
    world.mulRotateYXZ(r);
    //そして座標変換
    VECTOR p[11];
    for (int i = 0; i < 11; i++) {
        p[i] = world * osqp[i];
        p[i] = gView * p[i];
        p[i] = gProj * p[i];
    }
    //描画
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
