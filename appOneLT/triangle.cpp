#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"
#include"segment.h"
#include "TRIANGLE.h"

void TRIANGLE::create(const VECTOR& p0, const VECTOR& p1, const VECTOR& p2)
{
    //���_�ʒu
    P[0] = p0; P[1] = p1; P[2] = p2;
    //��(Side)�x�N�g��
    S[0] = p1 - p0; S[1] = p2 - p1; S[2] = p0 - p2;
    //�@���x�N�g��
    N = cross(S[0], S[1]).normalize();
    //���ʂ̎� ax+by+cz+d=0 �̂�
    D = dot(-N, p0);
}

bool TRIANGLE::contains(const VECTOR& p)
{
    //���̎O�p�`���u�^��v���猩�����A�_(p.x p.z)���܂܂�Ă��邩�H
    bool flag = true;
    for (int i = 0; flag == true && i < 3; i++) {
        VECTOR pv = p - P[i];
        //�Ӄx�N�g���Ƃ��x�N�g���́u�O�ς������v
        float y = S[i].z * pv.x - S[i].x * pv.z;
        if (y < 0) {
            //�܂܂�Ă��Ȃ�
            flag = false;
        }
    }
    //�܂�ł���
    if (flag) {
        Color = COLOR(255, 0, 0);
    }
    return flag;
}

float TRIANGLE::getY(const VECTOR& p)
{
    //�ip.x p.z�j��ʂ̎��ɓ��Ă͂�p.y�����߂�
    return (-N.x * p.x - N.z * p.z - D) / N.y;
}

void TRIANGLE::draw()
{
    //�֊s
    for (int i = 0; i < 3; i++) {
        segment(P[i], P[(i+1)%3], COLOR(255,255,255), 3);
    }
    //�|���S��
    VECTOR p[3];
    for (int i = 0; i < 3; i++) {
        p[i] = gView * P[i];
        p[i] = gProj * p[i];
    }
    triangle3D(p[0], p[1], p[2], COLOR(255,128,0), Color, Color);
    //�`���A�_���܂�ł��Ȃ��Ƃ��̐F�ɖ߂�
    Color = COLOR(64, 128, 64);
}
