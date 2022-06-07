#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"
#include"mathUtil.h"
static const int numAng = 40;//角数。偶数であること。
static const int numVtx = numAng * 4;//全頂点数
static float radius = 1.0f;
static VECTOR op[numVtx];
static float h[2] = { 1.0f,0.0f };
static float bright[numVtx];
static VECTOR light(0.f, 0.9f, 0.9f);
static float ambient = 0.3f;

void createCylinder()
{
    light.normalize();
    //上から下に円を半径rを変えながらnumAng/2+1の数だけ用意する
    float angle = 3.1415926f / (numAng / 2);
    for (int j = 0; j < 2; j++) {
        float y = h[j];
        //円の座標
        for (int i = 0; i < numAng; i++) {
            int k = numAng * j + i;
            op[k].set(Cos(angle * i), y, -Sin(angle * i));
            bright[k] = dot(light, VECTOR(op[k].x,0,op[k].z));
            if (bright[k] < ambient)bright[k] = ambient;
        }
    }
    for (int i = 0; i < numAng; i++) {
        int k = numAng * 2 + i;
        op[k].set(Cos(angle * i), h[0], -Sin(angle * i));
        bright[k] = dot(light, VECTOR(0,1,0));
        if (bright[k] < ambient)bright[k] = ambient;
    }
    for (int i = 0; i < numAng; i++) {
        int k = numAng * 3 + i;
        op[k].set(Cos(angle * i), h[1], Sin(angle * i));
        bright[k] = dot(light, VECTOR(0, -1, 0));
        if (bright[k] < ambient)bright[k] = ambient;
    }
}
void cylinder(const VECTOR& t, const COLOR& col, float s)
{
    gWorld.identity();
    gWorld.mulTranslate(t);
    gWorld.mulScaling(s, 1, s);
    //そして座標変換
    VECTOR p[numVtx];
    for (int i = 0; i < numVtx; i++) {
        p[i] = gWorld * op[i];
        p[i] = gView * p[i];
        p[i] = gProj * p[i];
    }
    //描画
    for (int i = 0; i < numAng; i++) {
        //四角形
        int a = i;
        int b = i + numAng;
        int c = (i + 1) % numAng == 0 ? i + 1 - numAng : i + 1;
        int d = (b + 1) % numAng == 0 ? b + 1 - numAng : b + 1;
        VECTOR m = p[b] - p[a];
        VECTOR n = p[c] - p[a];
        if (m.x * n.y - m.y * n.x >= 0) {
            triangle3D(p[a], p[b], p[c], col * bright[a], col * bright[b], col * bright[c]);
            triangle3D(p[c], p[b], p[d], col * bright[c], col * bright[b], col * bright[d]);
        }
    }
    for (int i = 0; i < numAng - 2; i++) {
        int a = 0 + numAng*2;
        int b = i + 1+numAng*2;
        int c = i + 2+numAng*2;
        //top
        triangle3D(p[a], p[b], p[c], col*bright[a], col*bright[b], col*bright[c]);
    }
    for (int i = 0; i < numAng - 2; i++) {
        int a = 0 + numAng * 3;
        int b = i + 1 + numAng * 3;
        int c = i + 2 + numAng * 3;
        //top
        triangle3D(p[a], p[b], p[c], col * bright[a], col * bright[b], col * bright[c]);
    }
}
