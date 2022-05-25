#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"
#include"mathUtil.h"
#include"segment.h"
//‘½Šp’Œ‚ğü•ª‚Æ‚·‚é
static const int numAng = 4;//Šp”B‹ô”‚Å‚ ‚é‚±‚ÆB
static const int numVtx = numAng * 2;//‘S’¸“_”
static float radius = 0.0005f;//’¼Œa‚Pƒ~ƒŠ
static float x[2] = { 0.0f,1.0f };
static VECTOR op[numVtx];
static VECTOR p[numVtx];
void createSegment()
{
    //Œ´“_‚©‚ç‰E‚ÖL‚Ñ‚éŠp’Œ‚ğì‚é
    float angle = 3.1415926f / (numAng / 2);
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < numAng; i++) {
            op[numAng * j + i].x = x[j];
            op[numAng * j + i].y = Cos(angle * i) * radius;
            op[numAng * j + i].z = Sin(angle * i) * radius;
        }
    }
}
void segment(const VECTOR& sp, const VECTOR& ep, const COLOR& col, float thickness)
{
    VECTOR v = ep - sp;
    float len = v.mag();
    float angleY = Atan2(-v.z, v.x);
    v.normalize();
    float angleZ = Acos(-v.y) - 1.57f;
    gWorld.identity();
    gWorld.mulTranslate(sp.x, sp.y, sp.z);
    gWorld.mulRotateY(angleY);
    gWorld.mulRotateZ(angleZ);
    gWorld.mulScaling(len, thickness, thickness);
    for (int i = 0; i < numVtx; i++) {
        p[i] = gWorld * op[i];
        p[i] = gView * p[i];
        p[i] = gProj * p[i];
    }
    //•`‰æ
    for (int i = 0; i < numAng; i++) {
        //‘¤–Ê‚Å‚ ‚élŠpŒ`
        int a = i;
        int b = i + numAng;
        int c = (i + 1) % numAng == 0 ? i + 1 - numAng : i + 1;
        int d = (b + 1) % numAng == 0 ? b + 1 - numAng : b + 1;
        triangle3D(p[a], p[b], p[c], col, col, col);
        triangle3D(p[c], p[b], p[d], col, col, col);
    }
}
