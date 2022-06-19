#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"
#include"mathUtil.h"
static const int numAng = 24;//äpêîÅBãÙêîÇ≈Ç†ÇÈÇ±Ç∆ÅB
static const int numVtx = numAng * (numAng / 2) + 2;//ëSí∏ì_êî
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
    //ñkã…Ç…Ç»ÇÈí∏ì_
    op[k].set(0, 1, 0);
    bright[k] = dot(light, op[k]);
    if (bright[k] < ambient)bright[k] = ambient;
    k++;
    //è„îºãÖ
    for (j = 0; j < (numAng / 4); j++) {
        float y = Cos(angle * (j + 1));
        float r = Sin(angle * (j + 1));
        //â~ÇÃç¿ïW
        for (int i = 0; i < numAng; i++) {
            op[k].set(Cos(angle * i) * r, y, Sin(angle * i) * r);
            //op[k]Çñ@ê¸Ç∆ÇµÇƒí∏ì_ÇÃñæÇÈÇ≥ÇåvéZ
            bright[k] = dot(light, op[k]);
            if (bright[k] < ambient)bright[k] = ambient;
            k++;
        }
    }
    //â∫îºãÖ
    for (j -= 1; j < (numAng / 2) -1; j++) {
        float y = Cos(angle * (j + 1));
        float r = Sin(angle * (j + 1));
        //â~ÇÃç¿ïW
        for (int i = 0; i < numAng; i++) {
            op[k].set(Cos(angle * i) * r, y, Sin(angle * i) * r);
            //op[k]Çñ@ê¸Ç∆ÇµÇƒí∏ì_ÇÃñæÇÈÇ≥ÇåvéZ
            bright[k] = dot(light, op[k]);
            if (bright[k] < ambient)bright[k] = ambient;
            k++;
        }
    }
    //ìÏã…Ç…Ç»ÇÈí∏ì_
    op[k].set(0, -1, 0);
    bright[k] = dot(light, op[k]);
    if (bright[k] < ambient)bright[k] = ambient;
}
void capsule(const VECTOR& t, const VECTOR& r, 
    const COLOR& col, float segStartPos, float segEndPos,float radius)
{
    VECTOR p[numVtx];
    //è„îºãÖ
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
    //â∫îºãÖ
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
    //ï`âÊ
    //è„äWìIÇ»ñ ÇÕéOäpå`Ç≈ï`âÊ
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
    //ë§ñ ìIÇ»ñ ÇÕéläpå`Ç≈ï`âÊ
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
    ////â∫äWìIÇ»ñ ÇÕéOäpå`Ç≈ï`âÊ
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
//    //è„îºãÖ
//    for (j = 0; j < (numAng / 4); j++) {
//        float y = Cos(angle * (j + 1));
//        float r = Sin(angle * (j + 1));
//        //â~ÇÃç¿ïW
//        for (int i = 0; i < numAng; i++) {
//            op[k].set(Cos(angle * i) * r, y, Sin(angle * i) * r);
//            //op[k]Çñ@ê¸Ç∆ÇµÇƒí∏ì_ÇÃñæÇÈÇ≥ÇåvéZ
//            bright[k] = dot(light, op[k]);
//            if (bright[k] < ambient)bright[k] = ambient;
//            op[k] *= radius;
//            op[k].y += ofst;
//            k++;
//        }
//    }
//    //â∫îºãÖ
//    for (j -= 1; j < (numAng / 2) - 1; j++) {
//        float y = Cos(angle * (j + 1));
//        float r = Sin(angle * (j + 1));
//        //â~ÇÃç¿ïW
//        for (int i = 0; i < numAng; i++) {
//            op[k].set(Cos(angle * i) * r, y, Sin(angle * i) * r);
//            //op[k]Çñ@ê¸Ç∆ÇµÇƒí∏ì_ÇÃñæÇÈÇ≥ÇåvéZ
//            bright[k] = dot(light, op[k]);
//            if (bright[k] < ambient)bright[k] = ambient;
//            op[k] *= radius;
//            op[k].y -= ofst;
//            k++;
//        }
//    }
//    //ñkã…Ç…Ç»ÇÈí∏ì_
//    op[k].set(0, radius + ofst, 0);
//    bright[k] = dot(light, op[k]);
//    if (bright[k] < ambient)bright[k] = ambient;
//    //ìÏã…Ç…Ç»ÇÈí∏ì_
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
//    //ÇªÇµÇƒç¿ïWïœä∑
//    VECTOR p[numVtx];
//    for (int i = 0; i < numVtx; i++) {
//        p[i] = gWorld * op[i];
//        p[i] = gView * p[i];
//        p[i] = gProj * p[i];
//    }
//    //ï`âÊ
//    //ë§ñ ìIÇ»ñ ÇÕéläpå`Ç≈ï`âÊ
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
//    //è„äWìIÇ»ñ ÇÕéOäpå`Ç≈ï`âÊ
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
//    //â∫äWìIÇ»ñ ÇÕéOäpå`Ç≈ï`âÊ
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
