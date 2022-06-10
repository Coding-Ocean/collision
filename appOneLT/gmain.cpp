#include"libOne.h"
#include"input_.h"
#include"view_proj.h"
#include"axis.h"
#include"segment.h"
#include"point.h"
struct SEGMENT {
    VECTOR sp;
    VECTOR ep;
    VECTOR v;
    void set(const VECTOR& startPos, const VECTOR& endPos) 
    {
        sp = startPos; 
        ep = endPos;
        v = ep - sp;
    }
    void set(const VECTOR& tran, const VECTOR& rot,const VECTOR& osp, const VECTOR& oep) 
    {
        gWorld.identity();
        gWorld.mulTranslate(tran);
        gWorld.mulRotateYXZ(rot);
        sp = gWorld * osp;
        ep = gWorld * oep;
        v = ep - sp;
    }
    void draw() 
    {
        segment(sp, ep, COLOR(255, 255, 255), 10);
    }
};
//�_�ƒ����̍ŒZ����
float calcPointLineDist//�ŒZ����
(
    const VECTOR& p,//�_ 
    const SEGMENT& s,
    VECTOR& mp, //�_���牺�낵�������̒[�_
    float& t //�x�N�g���W��
) 
{
    float segMagSq = s.v.magSq();
    t = 0.0f;
    if (segMagSq > 0.0f) {
        t = dot(s.v, p - s.sp) / segMagSq;
    }
    mp = s.sp + s.v * t;
    return (mp - p).mag();
}
// �_�Ɛ����̍ŒZ����
float calcPointSegmentDist//�ŒZ����
(
    const VECTOR& p, //�_
    const SEGMENT& s,
    VECTOR& mp, //�ŒZ�����ƂȂ�[�_
    float& t //�x�N�g���W��
) 
{
    // �����̒���len�A�����̑��̍��Wmp�y��t���Z�o
    float len = calcPointLineDist(p, s, mp, t);
    if (t < 0.0f) {
        mp = s.sp;
        return (p - mp).mag();
    }
    if (t > 1.0f) {
        mp = s.ep;
        return(p - mp).mag();
    }
    return len;
}
//�����ƒ����̍ŒZ����
float calcLineLineDist
(
    const SEGMENT& s1,
    const SEGMENT& s2,
    VECTOR& mp1, 
    VECTOR& mp2,
    float& t1, 
    float& t2
) 
{
    // 2���������s
    if (cross(s1.v, s2.v).magSq() < 0.000001f) {
        //����1�̎n�_�ƒ���2�̍ŒZ�����̖��ɋA��
        float len = calcPointLineDist(s1.sp, s2, mp2, t2);
        mp1 = s1.sp;
        t1 = 0.0f;
        return len;
    }

    // 2�����͂˂���֌W
    float dsv1sv2 = dot(s1.v, s2.v);
    float sv1magSq = s1.v.magSq();
    float sv2magSq = s2.v.magSq();
    VECTOR sp2sp1 = s1.sp - s2.sp;
    t1 = (dsv1sv2 * dot(s2.v,sp2sp1) - sv2magSq * dot(s1.v,sp2sp1))
        / (sv1magSq * sv2magSq - dsv1sv2 * dsv1sv2);
    mp1 = s1.sp + s1.v * t1;
    t2 = dot(s2.v, mp1 - s2.sp) / sv2magSq;
    mp2 = s2.sp + s2.v * t2;
    return (mp2 - mp1).mag();
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
    const SEGMENT& s1,
    const SEGMENT& s2,
    VECTOR& mp1, 
    VECTOR& mp2, 
    float& t1, 
    float& t2
) {
    float len = 0;

    /*
    // sv1���k�ނ��Ă���H
    if (sv1.magSq() < 0.000001f) {
        // sv2���k�ށH
        if (sv2.magSq() < 0.000001f) {
            // �_�Ɠ_�̋����̖��ɋA��
            len = (sp2 - sp1).mag();
            mp1 = sp1;
            mp2 = sp2;
            t1 = t2 = 0.0f;
            segment(mp1, mp2, COLOR(255, 255, 255), 10);
            return len;
        }
        else {
            // sp1��sv2�̍ŒZ���ɋA��
            len = calcPointSegmentDist(sp1, sp2, ep2, mp2, t2);
            mp1 = sp1;
            t1 = 0.0f;
            clamp01(t2);
            segment(mp1, mp2, COLOR(255, 255, 255), 10);
            return len;
        }
    }
    // sv2���k�ނ��Ă���H
    else if (sv2.magSq() < 0.000001f) {
        // sp2��sv1�̍ŒZ���ɋA��
        float len = calcPointSegmentDist(sp2, sp1, ep1, mp1, t1);
        mp2 = sp2;
        clamp01(t1);
        t2 = 0.0f;
        segment(mp1, mp2, COLOR(255, 255, 255), 10);
        return len;
    }
    */

    /* �������m */

    // 2�����Ԃ̍ŒZ���������߂ĉ���t1,t2�����߂�
    len = calcLineLineDist(s1, s2, mp1, mp2, t1, t2);
    if (0.0f <= t1 && t1 <= 1.0f &&
        0.0f <= t2 && t2 <= 1.0f) {
        segment(mp1, mp2, COLOR(255, 255, 0), 10);
        return len;
    }

    // �����̑����O�ɂ��鎖�������i���s�ł������ɗ��邱�Ƃ����j----------------
    // t2��0�`1�ɃN�����v����sv1�ɐ������~�낷
    clamp01(t2);
    mp2 = s2.sp + s2.v * t2;
    len = calcPointSegmentDist(mp2, s1, mp1, t1);
    if (0.0f <= t1 && t1 <= 1.0f) {
        segment(mp1, mp2, COLOR(0, 0, 255), 10);
        return len;
    }
    // t1��0�`1�ɃN�����v����sv2�ɐ������~�낷
    clamp01(t1);
    mp1 = s1.sp + s1.v * t1;
    len = calcPointSegmentDist(mp1, s2, mp2, t2);
    if (0.0f <= t2 && t2 <= 1.0f) {
        segment(mp1, mp2, COLOR(0, 255, 0), 10);
        return len;
    }

    // �o���̒[�_���ŒZ�Ɣ���-----------------------------------------------
    clamp01(t1);
    mp1 = s1.sp + s1.v * t1;
    segment(mp1, mp2, COLOR(255, 0, 255), 10);
    return (mp2 - mp1).mag();
}

void gmain() {
    window(1920, 1080, full);
    hideCursor();
    createSegment();
    createPoint();
    //-----------------------------------------------------------------------
    //�����P�̃I���W�i���|�W�V����
    VECTOR osp1(0, 0.4f, 0);//original start point
    VECTOR oep1(0, -0.4f, 0);//original end point
    //�����P�̍��p�ϊ���̃|�W�V����
    SEGMENT s1;
    VECTOR seg1Tran(0.0f, 0.0f, 0.0f);
    VECTOR seg1Rot;
    //-----------------------------------------------------------------------
    //����2�̃I���W�i���|�W�V����
    VECTOR osp2(0, 0.4f, 0);//original start point
    VECTOR oep2(0, -0.4f, 0);//original end point
    //����2�̍��W�ϊ���̃|�W�V����
    SEGMENT s2;
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
            s1.set(seg1Tran, seg1Rot, osp1, oep1);
        }
        //����2�𓮂���---------------------------------------------------------
        {
            if (operateObjSw == 0) {
                input(seg2Tran, seg2Rot, speed*delta);
            }
            s2.set(seg2Tran, seg2Rot, osp2, oep2);
        }
        //�����蔻��----------------------------------------------------------
        {
            VECTOR mp1;
            VECTOR mp2;
            float t1;
            float t2;
            float dist = 0;
            //dist = calcPointLineDist(sp1, sp2, ep2, mp2, t2);
            //dist = calcPointSegmentDist(sp1, sp2, ep2, mp2, t2);
            //dist = calcLineLineDist(sp1, ep1, sp2, ep2, mp1, mp2, t1, t2);
            dist = calcSegmentSegmentDist(s1, s2, mp1, mp2, t1, t2);
            print(dist);
            //if (flag) cylinderColor = transRed;
            //else cylinderColor = transWhite;
        }
        //�`��----------------------------------------------------------------
        {
            //��
            if(dispAxisFlag)axis(white, 1);
            //�����P
            s1.draw();
            //�����Q
            s2.draw();
            //�e�L�X�g���
            //float size = 30;
            //textSize(size);
            //float colL = 10;//��̎n�܂�
            //float rowH = size + 10;//�s�̍���
            //int num = 0;//�s�ԍ�
            //fill(COLOR(255, 255, 255));
            //text((let)"", colL, ++num * rowH);
        }
    }
}
