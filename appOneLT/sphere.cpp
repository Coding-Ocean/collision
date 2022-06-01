#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"
#include"mathUtil.h"
static const int numAng = 40;//�p���B�����ł��邱�ƁB
static const int numVtx = numAng * (numAng / 2 - 1) + 2;//�S���_��
static VECTOR op[numVtx];
static float bright[numVtx];
static VECTOR light(0.3f, 0.3f, 1);
static float ambient = 0.3f;
void createSphere()
{
    light.normalize();
    float angle = 3.1415926f / (numAng / 2);
    int k = 0;
    for (int j = 0; j < numAng / 2-1; j++) {
        float y = Cos(angle * (j+1));
        float r = Sin(angle * (j+1));
        //�~�̍��W
        for (int i = 0; i < numAng; i++) {
            k = numAng * j + i;
            op[k].set(Cos(angle * i) * r, y, Sin(angle * i) * r);
            //op[k]��@���Ƃ��Ē��_�̖��邳���v�Z
            bright[k] = dot(light, op[k]);
            if (bright[k] < ambient)bright[k] = ambient;
        }
    }
    //�k�ɂɂȂ钸�_
    k++;
    op[k].set(0,1,0);
    bright[k] = dot(light, op[k]);
    if (bright[k] < ambient)bright[k] = ambient;
    ////��ɂɂȂ钸�_
    k++;
    op[k].set(0,-1,0);
    bright[k] = dot(light, op[k]);
    if (bright[k] < ambient)bright[k] = ambient;
}
void sphere(const VECTOR& t, const COLOR& col, float d)
{
    gWorld.identity();
    gWorld.mulTranslate(t);
    gWorld.mulScaling(d, d, d);
    //�����č��W�ϊ�
    VECTOR p[numVtx];
    for (int i = 0; i < numVtx; i++) {
        p[i] = gWorld * op[i];
        p[i] = gView * p[i];
        p[i] = gProj * p[i];
    }
    //�`��
    for (int i = 0; i < numVtx - numAng-2; i++) {
        //�l�p�`
        int a = i;
        int b = i + numAng;
        int c = (i + 1) % numAng == 0 ? i + 1 - numAng : i + 1;
        int d = (b + 1) % numAng == 0 ? b + 1 - numAng : b + 1;
        VECTOR m = p[b] - p[a];
        VECTOR n = p[c] - p[a];
        if (m.x * n.y - m.y * n.x <= 0) {
            triangle3D(p[a], p[b], p[c], col * bright[a], col * bright[b], col * bright[c]);
            triangle3D(p[c], p[b], p[d], col * bright[c], col * bright[b], col * bright[d]);
        }
    }
    for (int i = 0; i < numAng; i++) {
        //�l�p�`
        int a = numVtx - 2;
        int b = i;
        int c = (i + 1) % numAng == 0 ? i + 1 - numAng : i + 1;
        VECTOR m = p[b] - p[a];
        VECTOR n = p[c] - p[a];
        if (m.x * n.y - m.y * n.x <= 0) {
            triangle3D(p[a], p[b], p[c], col * bright[a], col * bright[b], col * bright[c]);
        }
    }
    for (int i = numVtx-numAng-2; i < numVtx; i++) {
        //�l�p�`
        int a = numVtx - 1;
        int b = i;
        int c = (i + 1) % numAng == 0 ? i + 1 - numAng : i + 1;
        VECTOR m = p[b] - p[a];
        VECTOR n = p[c] - p[a];
        if (m.x * n.y - m.y * n.x >= 0) {
            triangle3D(p[a], p[b], p[c], col * bright[a], col * bright[b], col * bright[c]);
        }
    }
}
