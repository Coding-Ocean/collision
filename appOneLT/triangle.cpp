#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"
#include"segment.h"
#include "TRIANGLE.h"

void TRIANGLE::create(const VECTOR& p0, const VECTOR& p1, const VECTOR& p2)
{
    //頂点位置
    P[0] = p0; P[1] = p1; P[2] = p2;
    //辺(Side)ベクトル
    S[0] = p1 - p0; S[1] = p2 - p1; S[2] = p0 - p2;
    //法線ベクトル
    N = cross(S[0], S[1]).normalize();
    //面の式 ax+by+cz+d=0 のｄ
    D = -N.x * p0.x - N.y * p0.y - N.z * p0.z;
}

bool TRIANGLE::contains(const VECTOR& p)
{
    //この三角形を「真上」から見た時、点(p.x p.z)が含まれているか。
    bool flag = true;
    for (int i = 0; flag == true && i < 3; i++) {
        VECTOR pv = p - P[i];
        //辺ベクトルとｐベクトルの「外積ｙ成分」
        float y = S[i].z * pv.x - S[i].x * pv.z;
        if (y < 0) {
            //この三角形には含まれていない
            flag = false;
        }
    }
    if (flag) {
        Color = COLOR(255, 160, 160);
    }
    else {
        Color = COLOR(64, 128, 64);
    }
    return flag;
}

float TRIANGLE::getY(const VECTOR& p)
{
    //（p.x p.z）を面の式に当てはめp.yを求める
    return (-N.x * p.x - N.z * p.z - D) / N.y;
}

void TRIANGLE::draw()
{
    for (int i = 0; i < 3; i++) {
        segment(P[i], P[(i+1)%3], COLOR(255,255,255), 6);
    }
    VECTOR p[3];
    for (int i = 0; i < 3; i++) {
        p[i] = gView * P[i];
        p[i] = gProj * p[i];
    }
    triangle3D(p[0], p[1], p[2], COLOR(255,128,0), Color, Color);
}
