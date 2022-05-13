#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"
void wall() 
{
    //１回ループするごとに以下の値で初期化
    VECTOR p[4] = {
        VECTOR(0, 0, 0),
        VECTOR(1, 0, 0),
        VECTOR(0, 1, 0),
        VECTOR(0, 0, 1),
    };
    //そして座標変換
    for (int i = 0; i < 4; i++) {
        p[i] = gView * p[i];
        p[i] = gProj * p[i];
    }
    //描画
    COLOR c;
    c.set(192, 192, 192, 80);
    triangle3D(p[0], p[1], p[2], c, c, c);
    c.set(255, 255, 255, 80);
    triangle3D(p[0], p[1], p[3], c, c, c);
    c.set(128, 128, 128, 80);
    triangle3D(p[0], p[2], p[3], c, c, c);
}
