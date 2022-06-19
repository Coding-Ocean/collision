#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"
#include"mathUtil.h"
static const int numAng = 24;//角数。偶数であること。
static const int numVtx = numAng * (numAng / 2) + 2;//全頂点数
static VECTOR op[numVtx];
static float bright[numVtx];
static VECTOR light(0.3f, 0.3f, 1);
static float ambient = 0.3f;
static float ofst = 0.4f;
void createCapsule()
{
    light.normalize();
    float radius = 0.2f;
    float angle = 3.1415926f / (numAng / 2);
    int j, k = 0;
    //北極になる頂点
    op[k].set(0, 1, 0);
    bright[k] = dot(light, op[k]);
    if (bright[k] < ambient)bright[k] = ambient;
    k++;
    //上半球
    for (j = 0; j < (numAng / 4); j++) {
        float y = Cos(angle * (j + 1));
        float r = Sin(angle * (j + 1));
        //円の座標
        for (int i = 0; i < numAng; i++) {
            op[k].set(Cos(angle * i) * r, y, Sin(angle * i) * r);
            //op[k]を法線として頂点の明るさを計算
            bright[k] = dot(light, op[k]);
            if (bright[k] < ambient)bright[k] = ambient;
            k++;
        }
    }
    //下半球
    for (j -= 1; j < (numAng / 2) -1; j++) {
        float y = Cos(angle * (j + 1));
        float r = Sin(angle * (j + 1));
        //円の座標
        for (int i = 0; i < numAng; i++) {
            op[k].set(Cos(angle * i) * r, y, Sin(angle * i) * r);
            //op[k]を法線として頂点の明るさを計算
            bright[k] = dot(light, op[k]);
            if (bright[k] < ambient)bright[k] = ambient;
            k++;
        }
    }
    //南極になる頂点
    op[k].set(0, -1, 0);
    bright[k] = dot(light, op[k]);
    if (bright[k] < ambient)bright[k] = ambient;
}
void capsule(const VECTOR& t, const VECTOR& r, 
    const COLOR& col, float segStartPos, float segEndPos,float radius)
{
    VECTOR p[numVtx];
    //上半球
    gWorld.identity();
    gWorld.mulTranslate(t);
    gWorld.mulRotateYXZ(r);
    gWorld.mulTranslate(0,segStartPos,0);
    gWorld.mulScaling(radius,radius,radius);
    for (int i = 0; i < numVtx/2; i++) {
        p[i] = gWorld * op[i];
        p[i] = gView * p[i];
        p[i] = gProj * p[i];
    }
    //下半球
    gWorld.identity();
    gWorld.mulTranslate(t);
    gWorld.mulRotateYXZ(r);
    gWorld.mulTranslate(0,segEndPos,0);
    gWorld.mulScaling(radius,radius,radius);
    for (int i = numVtx/2; i < numVtx; i++) {
        p[i] = gWorld * op[i];
        p[i] = gView * p[i];
        p[i] = gProj * p[i];
    }
    //描画
    //上蓋的な面は三角形で描画
    for (int i = 0; i < numAng; i++) {
        int a = 0;
        int b = i+1;
        int c = (i+1) % numAng == 0 ? i + 2 - numAng : i + 2;
        VECTOR m = p[b] - p[a];
        VECTOR n = p[c] - p[a];
        if (m.x * n.y - m.y * n.x <= 0) {
            triangle3D(p[a], p[b], p[c], col * bright[a], col * bright[b], col * bright[c]);
        }
    }
    //側面的な面は四角形で描画
    for (int i = 0; i < numVtx - numAng - 2; i++) {
        int a = i+1;
        int b = a + numAng;
        int c = (i+1) % numAng == 0 ? b + 1 - numAng : b + 1;
        int d = (i+1) % numAng == 0 ? a + 1 - numAng : a + 1;
        VECTOR m = p[b] - p[a];
        VECTOR n = p[c] - p[a];
        if (m.x * n.y - m.y * n.x <= 0) 
        {
            triangle3D(p[a], p[b], p[c], col * bright[a], col * bright[b], col * bright[c]);
            triangle3D(p[a], p[c], p[d], col * bright[a], col * bright[c], col * bright[d]);
        }
    }
    ////下蓋的な面は三角形で描画
    for (int i = numVtx - numAng - 1; i < numVtx-1; i++) {
        int a = numVtx - 1;
        int b = i;
        int c = i % numAng == 0 ? i + 1 - numAng : i + 1;
        VECTOR m = p[b] - p[a];
        VECTOR n = p[c] - p[a];
        if (m.x * n.y - m.y * n.x >= 0) {
            triangle3D(p[a], p[b], p[c], col * bright[a], col * bright[b], col * bright[c]);
        }
    }
}
//void createCapsule()
//{
//    light.normalize();
//    float radius = 0.2f;
//    float angle = 3.1415926f / (numAng / 2);
//    int j, k = 0;
//    //上半球
//    for (j = 0; j < (numAng / 4); j++) {
//        float y = Cos(angle * (j + 1));
//        float r = Sin(angle * (j + 1));
//        //円の座標
//        for (int i = 0; i < numAng; i++) {
//            op[k].set(Cos(angle * i) * r, y, Sin(angle * i) * r);
//            //op[k]を法線として頂点の明るさを計算
//            bright[k] = dot(light, op[k]);
//            if (bright[k] < ambient)bright[k] = ambient;
//            op[k] *= radius;
//            op[k].y += ofst;
//            k++;
//        }
//    }
//    //下半球
//    for (j -= 1; j < (numAng / 2) - 1; j++) {
//        float y = Cos(angle * (j + 1));
//        float r = Sin(angle * (j + 1));
//        //円の座標
//        for (int i = 0; i < numAng; i++) {
//            op[k].set(Cos(angle * i) * r, y, Sin(angle * i) * r);
//            //op[k]を法線として頂点の明るさを計算
//            bright[k] = dot(light, op[k]);
//            if (bright[k] < ambient)bright[k] = ambient;
//            op[k] *= radius;
//            op[k].y -= ofst;
//            k++;
//        }
//    }
//    //北極になる頂点
//    op[k].set(0, radius + ofst, 0);
//    bright[k] = dot(light, op[k]);
//    if (bright[k] < ambient)bright[k] = ambient;
//    //南極になる頂点
//    k++;
//    op[k].set(0, -radius - ofst, 0);
//    bright[k] = dot(light, op[k]);
//    if (bright[k] < ambient)bright[k] = ambient;
//}
//void capsule(const VECTOR& t, const VECTOR& r, const COLOR& col)
//{
//    gWorld.identity();
//    gWorld.mulTranslate(t);
//    gWorld.mulRotateYXZ(r);
//    //そして座標変換
//    VECTOR p[numVtx];
//    for (int i = 0; i < numVtx; i++) {
//        p[i] = gWorld * op[i];
//        p[i] = gView * p[i];
//        p[i] = gProj * p[i];
//    }
//    //描画
//    //側面的な面は四角形で描画
//    for (int i = 0; i < numVtx - numAng - 2; i++) {
//        int a = i;
//        int b = i + numAng;
//        int c = (i + 1) % numAng == 0 ? i + 1 - numAng : i + 1;
//        int d = (b + 1) % numAng == 0 ? b + 1 - numAng : b + 1;
//        VECTOR m = p[b] - p[a];
//        VECTOR n = p[c] - p[a];
//        if (m.x * n.y - m.y * n.x <= 0)
//        {
//            triangle3D(p[a], p[b], p[c], col * bright[a], col * bright[b], col * bright[c]);
//            triangle3D(p[c], p[b], p[d], col * bright[c], col * bright[b], col * bright[d]);
//        }
//    }
//    //上蓋的な面は三角形で描画
//    for (int i = 0; i < numAng; i++) {
//        int a = numVtx - 2;
//        int b = i;
//        int c = (i + 1) % numAng == 0 ? i + 1 - numAng : i + 1;
//        VECTOR m = p[b] - p[a];
//        VECTOR n = p[c] - p[a];
//        if (m.x * n.y - m.y * n.x <= 0) {
//            triangle3D(p[a], p[b], p[c], col * bright[a], col * bright[b], col * bright[c]);
//        }
//    }
//    //下蓋的な面は三角形で描画
//    for (int i = numVtx - numAng - 2; i < numVtx; i++) {
//        int a = numVtx - 1;
//        int b = i;
//        int c = (i + 1) % numAng == 0 ? i + 1 - numAng : i + 1;
//        VECTOR m = p[b] - p[a];
//        VECTOR n = p[c] - p[a];
//        if (m.x * n.y - m.y * n.x >= 0) {
//            triangle3D(p[a], p[b], p[c], col * bright[a], col * bright[b], col * bright[c]);
//        }
//    }
//}
