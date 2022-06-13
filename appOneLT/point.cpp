#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"
#include"mathUtil.h"
#include"point.h"
static const int numAng = 6;//角数。偶数であること。
static const int numVtx = numAng * (numAng / 2 + 1);//全頂点数
static float radius = 0.0005f;
static VECTOR op[numVtx];
void createPoint()
{
    //上から下に円を半径rを変えながらnumAng/2+1の数だけ用意する
    float angle = 3.1415926f / (numAng / 2);
    for (int j = 0; j < numAng / 2 + 1; j++) {
        float y = Cos(angle * j) * radius;
        float r = Sin(angle * j) * radius;
        //円の座標
        for (int i = 0; i < numAng; i++) {
            op[numAng * j + i].set(Cos(angle * i) * r, y, Sin(angle * i) * r);
        }
    }
}
void point(const VECTOR& t, const COLOR& col, float d)
{
    gWorld.identity();
    gWorld.mulTranslate(t);
    gWorld.mulScaling(d, d, d);
    //そして座標変換
    VECTOR p[numVtx];
    for (int i = 0; i < numVtx; i++) {
        p[i] = gWorld * op[i];
        p[i] = gView * p[i];
        p[i] = gProj * p[i];
    }
    //描画
    for (int i = 0; i < numVtx - numAng; i++) {
        //四角形
        int a = i;
        int b = i + numAng;
        int c = (i + 1) % numAng == 0 ? i + 1 - numAng : i + 1;
        int d = (b + 1) % numAng == 0 ? b + 1 - numAng : b + 1;
        triangle3D(p[a], p[b], p[c], col, col, col);
        triangle3D(p[c], p[b], p[d], col, col, col);
    }
}
