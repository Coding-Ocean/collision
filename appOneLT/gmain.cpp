#include"libOne.h"
#include"input_.h"
#include"view_proj.h"
#include"axis.h"
#include"segment.h"
#include"point.h"
#include"sphere.h"
#include"cylinder.h"

bool intersects(
    const VECTOR& p, float radius,
    const VECTOR& sp, const VECTOR& ep, float segLen
) 
{
    VECTOR v(0, 1, 0);
    float cylinderLen = 1.0f;
    VECTOR sv = (ep - sp).normalize();
    float radiusSq = radius * radius;

    VECTOR pv;

    VECTOR _cross = cross(v, sv);
    float len = _cross.sqMag();
    if (len < 0.000001f) {
        //���s�x�N�g���̂Ƃ�
        VECTOR v_ = cross(sp - p, v);
        //*n = p - P[i];
        if (v_.magSq() < radiusSq) {
            if (sv.y > 0) {
                if (sp.y + segLen > p.y && sp.y < p.y + cylinderLen) {
                    return true;
                }
            }
            else {
                if (sp.y > p.y && sp.y - segLen < p.y + cylinderLen) {
                    return true;
                }
            }
        }
    }
    else {
        float dv = dot(v, sv);
        float dA = dot(sp - p, v);
        float dB = dot(sp - p, sv);
        float lenA = (dA - dB * dv) / (1.0f - dv * dv);
        float lenB = (dB - dA * dv) / (dv * dv - 1.0f);
        VECTOR qA = p + v * lenA;
        VECTOR qB = sp + sv * lenB;

        segment(p, p + v * lenA, COLOR(255, 255, 0), 10);
        segment(sp, sp + sv * lenB, COLOR(255, 0, 0), 8);
        print(lenA);
        print(lenB);
        point(qA, COLOR(255, 255, 0), 20);
        point(qB, COLOR(255, 0, 0), 20);
        pv = qA - qB;
        segment(qB, qA, COLOR(255, 0, 255), 10);

        if (pv.magSq() < radiusSq) 
        {
            if (0 <= lenA && lenA <= cylinderLen) 
            {
                if (0 <= lenB && lenB <= segLen) 
                {
                    return true;
                }
            }
        }
    }

    return false;
}

// �_�ƒ����̍ŒZ����
float calcPointLineDist//�ŒZ����
(
    const VECTOR& p,//�_ 
    const VECTOR& sp,//�n�_
    const VECTOR& ep,//�I�_
    VECTOR& h, //�_���牺�낵�������̒[�_
    float& t //�x�N�g���W��
) 
{
    //�����̒���
    VECTOR sv = ep - sp;
    float segMagSq = sv.magSq();
    //�x�N�g���W��
    t = 0.0f;
    if (segMagSq > 0.0f) {
        t = dot(sv, p - sp) / segMagSq;
    }
    //�����̒[�_
    h = sp + t * sv;

    //segment(p, h, COLOR(0, 255, 255), 10);
    //�Ȃ񂿂���Ē�����`��
    //segment(sp - sv * 3, ep + sv * 3, COLOR(255, 255, 255), 10);
    
    return (h - p).mag();
}
// �_�Ɛ����̍ŒZ����
float calcPointSegmentDist//�ŒZ����
(
    const VECTOR& p, //�_
    const VECTOR& sp,//�����n�_
    const VECTOR& ep,//�����I�_
    VECTOR& h, //�ŒZ�����ƂȂ�[�_
    float& t //�x�N�g���W��
) {
    // �����̒����A�����̑��̍��W�y��t���Z�o
    float len = calcPointLineDist(p, sp, ep, h, t);

    if (dot(p - sp, ep - sp) < 0) {
        //segment(sp, p, COLOR(255, 0, 0), 10);
        
        h = sp;
        return (p - sp).mag();
    }
    else if (dot(p - ep, sp - ep) < 0) {
        //segment(ep, p, COLOR(255, 0, 0), 10);
        
        h = ep;
        return(p - ep).mag();
    }
    
    //segment(h, p, COLOR(255, 0, 0), 10);
    
    return len;
}
//�����ƒ����̍ŒZ����
float calcLineLineDist
(
    const VECTOR& sp1,//����1�n�_
    const VECTOR& ep1,//����1�I�_
    const VECTOR& sp2,//����2�n�_
    const VECTOR& ep2,//����2�I�_
    VECTOR& p1, 
    VECTOR& p2,
    float& t1, 
    float& t2
) 
{
    VECTOR sv1 = ep1 - sp1;
    VECTOR sv2 = ep2 - sp2;

    // 2���������s�H
    //if (cross(sv1,sv2).magSq()< 0.000001f) {

    //    //����1�̎n�_�ƒ���2�̍ŒZ�����̖��ɋA��
    //    float len = calcPointLineDist(sp1, sp2, ep2, p2, t2);
    //    p1 = sp1;
    //    t1 = 0.0f;

    //    return len;
    //}

    // 2�����˂���̊֌W
    float dsv1sv2 = dot(sv1, sv2);
    float sv1magSq = sv1.magSq();
    float sv2magSq = sv2.magSq();
    VECTOR sp2sp1 = sp1 - sp2;
    t1 = (dsv1sv2 * dot(sv2,sp2sp1) - sv2magSq * dot(sv1,sp2sp1))
        / (sv1magSq * sv2magSq - dsv1sv2 * dsv1sv2);
    p1 = sp1 + sv1 * t1;
    t2 = dot(sv2, p1 - sp2) / sv2magSq;
    p2 = sp2 + sv2 * t2;

    //segment(p1, p2, COLOR(255, 0, 255), 10);

    return (p2 - p1).mag();
}

// 0�`1�̊ԂɃN�����v
void clamp01(float& v) {
    if (v < 0.0f)
        v = 0.0f;
    else if (v > 1.0f)
        v = 1.0f;
}
// 2�����̍ŒZ����
float calcSegmentSegmentDist
(
    const VECTOR& sp1,//����1�n�_
    const VECTOR& ep1,//����1�I�_
    const VECTOR& sp2,//����2�n�_
    const VECTOR& ep2,//����2�I�_
    VECTOR& p1, 
    VECTOR& p2, 
    float& t1, 
    float& t2
) {
    VECTOR sv1 = ep1 - sp1;
    VECTOR sv2 = ep2 - sp2;
    /*
    // S1���k�ނ��Ă���H
    if (sv1.magSq() < 0.000001f) {
        // S2���k�ށH
        if (sv2.magSq() < 0.000001f) {
            // �_�Ɠ_�̋����̖��ɋA��
            float len = (sp2 - sp1).mag();
            p1 = sp1;
            p2 = sp2;
            t1 = t2 = 0.0f;
            segment(p1, p2, COLOR(255, 255, 255), 12);
            return len;
        }
        else {
            // S1�̎n�_��S2�̍ŒZ���ɋA��
            float len = calcPointSegmentDist(sp1, sp2, ep2, p2, t2);
            p1 = sp1;
            t1 = 0.0f;
            clamp01(t2);
            segment(p1, p2, COLOR(255, 255, 255), 12);
            return len;
        }
    }

    // S2���k�ނ��Ă���H
    else if (sv2.magSq() < 0.000001f) {
        // S2�̎n�_��S1�̍ŒZ���ɋA��
        float len = calcPointSegmentDist(sp2, sp1, ep1, p1, t1);
        p2 = sp2;
        clamp01(t1);
        t2 = 0.0f;
        segment(p1, p2, COLOR(255, 255, 255), 12);
        return len;
    }
    */


    /* �������m */

    // 2���������s�������琂���̒[�_�̈��P1�ɉ�����
    if (cross(sv1, sv2).magSq() < 0.000001f) {
        t1 = 0.0f;
        p1 = sp1;
        float len = calcPointSegmentDist(sp1, sp2, ep2, p2, t2);
        if (0.0f <= t2 && t2 <= 1.0f) {
            segment(p1, p2, COLOR(255, 0, 255), 12);

            return len;
        }
    }
    else {
        // �����͂˂���̊֌W
        // 2�����Ԃ̍ŒZ���������߂ĉ���t1,t2�����߂�
        float len = calcLineLineDist(sp1, ep1, sp2, ep2, p1, p2, t1, t2);
        if (
            0.0f <= t1 && t1 <= 1.0f &&
            0.0f <= t2 && t2 <= 1.0f
            ) {
            segment(p1, p2, COLOR(255, 255, 0), 12);
            return len;
        }
    }

    // �����̑����O�ɂ��鎖������
    // S1����t1��0�`1�̊ԂɃN�����v���Đ������~�낷
    clamp01(t1);
    p1 = sp1 + sv1 * t1;
    float len = calcPointSegmentDist(p1, sp2, ep2, p2, t2);
    if (0.0f <= t2 && t2 <= 1.0f) {
        segment(p1, p2, COLOR(255, 0, 0), 12);
        return len;
    }

    // S2�����O�������̂�S2�����N�����v�AS1�ɐ������~�낷
    clamp01(t2);
    p2 = sp2 + sv2 * t2;
    len = calcPointSegmentDist(p2, sp1, ep1, p1, t1);
    if (0.0f <= t1 && t1 <= 1.0f) {
        segment(p1, p2, COLOR(0, 255, 0), 12);
        return len;
    }

    // �o���̒[�_���ŒZ�Ɣ���
    clamp01(t1);
    p1 = sp1 + sv1 * t1;
    segment(p1, p2, COLOR(0, 0,255), 12);
    return (p2 - p1).mag();
}

void gmain() {
    window(1920, 1080, full);
    hideCursor();
    createSegment();
    createPoint();
    createCylinder();
    createSphere();
    //-----------------------------------------------------------------------
    //�����P�̃I���W�i���|�W�V����
    VECTOR osp1(0, 0.4f, 0);//original start point
    VECTOR oep1(0, -0.4f, 0);//original end point
    //�����P�̍��p�ϊ���̃|�W�V����
    VECTOR sp1;//start point
    VECTOR ep1;//end point
    VECTOR seg1Tran(0.0f, 0.0f, 0.0f);
    VECTOR seg1Rot;
    //-----------------------------------------------------------------------
    //����2�̃I���W�i���|�W�V����
    VECTOR osp2(0, 0.4f, 0);//original start point
    VECTOR oep2(0, -0.4f, 0);//original end point
    //����2�̍��W�ϊ���̃|�W�V����sp,ep
    VECTOR sp2;//start point
    VECTOR ep2;//end point
    VECTOR seg2Tran(0.5f, 0.0f, 0.0f);//�Z�O�����g�̈ړ��p segment translate
    VECTOR seg2Rot;//�Z�O�����g�̉�]�p segment rotate
    //�ړ���]�X�s�[�h
    MATRIX world;
    float speed = 0.3f;
    //�\���t���b�O
    bool dispAxisFlag = false;
    //�ړ���]������I�u�W�F�N�g�̑I��
    int operateObjSw = 0;
    //���̑�------------------------------------------------------------------
    //�F
    COLOR white(220, 220, 220);
    COLOR transRed(255, 0, 0, 150);
    COLOR transWhite(255, 255, 255,180);
    COLOR cylinderColor = transWhite;
    //�v���W�F�N�V�����s�������Ă���
    createProj();
    //�f���^�^�C��������
    initDeltaTime();
    //���C�����[�v-------------------------------------------------------------
    while (notQuit) {
        //�f���^�^�C���ݒ�
        setDeltaTime();
        //�X�V���̃f�[�^��\�����邽�߁A�����ŃN���A
        clear(30,80,180);
        //�J�����s����X�V
        updateView();
        //�\���ؑցA����I�u�W�F�N�g�؂�ւ�--------------------------------------
        {
            if (isTrigger(KEY_X)) { dispAxisFlag = !dispAxisFlag; }
            if (isTrigger(KEY_Z)) { operateObjSw = 1 - operateObjSw; }
            if (isTrigger(KEY_R)) {
                seg2Tran.set(0.5f, 0.0f, 0.0f);
                seg2Rot.set(0, 0, 0);
                seg1Tran.set(0, 0, 0);
                seg1Rot.set(0, 0, 0);
            }
        }
        //����1�𓮂���-----------------------------------------------------------
        {
            if (operateObjSw == 1) {
                input(seg1Tran, seg1Rot, speed*delta);
            }
            world.identity();
            world.mulTranslate(seg1Tran);
            world.mulRotateYXZ(seg1Rot);
            sp1 = world * osp1;
            ep1 = world * oep1;
        }
        //����2�𓮂���---------------------------------------------------------
        {
            if (operateObjSw == 0) {
                input(seg2Tran, seg2Rot, speed*delta);
            }
            world.identity();
            world.mulTranslate(seg2Tran);
            world.mulRotateYXZ(seg2Rot);
            sp2 = world * osp2;
            ep2 = world * oep2;
        }
        //�����蔻��----------------------------------------------------------
        {
            VECTOR h1;
            VECTOR h2;
            float t1;
            float t2;
            float dist = 0;
            //dist = calcPointLineDist(sp1, sp2, ep2, h2, t2);
            //dist = calcPointSegmentDist(sp1, sp2, ep2, h2, t2);
            //dist = calcLineLineDist(sp1, ep1, sp2, ep2, h1, h2, t1, t2);
            dist = calcSegmentSegmentDist(sp1, ep1, sp2, ep2, h1, h2, t1, t2);
            print(dist);
            //if (flag) cylinderColor = transRed;
            //else cylinderColor = transWhite;
        }
        //�`��----------------------------------------------------------------
        {
            //��
            if(dispAxisFlag)axis(white, 1);
            //����
            segment(sp2, ep2, white, 10);
            //point(sp2, white, 10);
            //point(ep2, white, 10);
            //
            segment(sp1, ep1, white, 10);
            
            //sphere(p, cylinderColor, radius);
            //cylinder(p, cylinderColor, radius);
            
            //�e�L�X�g���
            float size = 30;
            textSize(size);
            float colL = 10;//��̎n�܂�
            float rowH = size + 10;//�s�̍���
            int num = 0;//�s�ԍ�
            fill(COLOR(255, 255, 255));
            text((let)"", colL, ++num * rowH);
        }
    }
}
