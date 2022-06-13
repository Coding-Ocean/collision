#include"libOne.h"
#include"input_.h"
#include"view_proj.h"
#include"axis.h"
#include"segment.h"
#include"point.h"
#include"capsule.h"
//�����N���X(�����o�f�[�^��public)
class SEGMENT {
public:
    VECTOR sp;//�n�_
    VECTOR ep;//�I�_
    VECTOR v;//�n�_����I�_�܂ł̃x�N�g��
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
float calcPointLineDist //�ŒZ����
(
    const VECTOR& p, //�_ 
    const SEGMENT& s, //�����i���������j
    VECTOR& mp, //�_���璼���ɉ��낵�������̒[�_
    float& t //�x�N�g���W��
) 
{
    t = 0.0f;
    float dsvsv = s.v.magSq();//dot(s.v, s.v)�Ɠ���
    if (dsvsv > 0.0f) {
        //t�́udot�œ��e����������s.v�̒����v�܂芄���ɂȂ�
        t = dot(s.v, p - s.sp) / dsvsv;
    }
    mp = s.sp + s.v * t;
    return (p - mp).mag();
}

//�_�Ɛ����̍ŒZ����
float calcPointSegmentDist//�ŒZ����
(
    const VECTOR& p, //�_
    const SEGMENT& s, //����
    VECTOR& mp, //�_��������܂ł̍ŒZ�����ƂȂ�_(�n�_��I�_�ɂȂ邱�Ƃ�����)
    float& t //�x�N�g���W��
) 
{
    //�Ƃ肠�����_���璼���܂ł̍ŒZ������mp,t�����߂Ă݂�
    float dist = calcPointLineDist(p, s, mp, t);
    //mp�������̊O�ɂ���(�n�_���)
    if (t < 0.0f) {
        //�_��������̎n�_�܂ł̋������ŒZ
        mp = s.sp;
        return (p - mp).mag();
    }
    //mp�������̊O�ɂ���(�I�_���)
    if (t > 1.0f) {
        //�_��������̏I�_�܂ł̋������ŒZ
        mp = s.ep;
        return(p - mp).mag();
    }
    //mp���������ɂ���
    //0 <= t <= 1
    return dist;
}

//�����ƒ����̍ŒZ����
float calcLineLineDist
(
    const SEGMENT& s1,//�����P
    const SEGMENT& s2,//�����Q
    VECTOR& mp1, //
    VECTOR& mp2,
    float& t1, 
    float& t2
) 
{
    //2���������s
    if (cross(s1.v, s2.v).magSq() < 0.000001f) {
        //����1�̎n�_���璼��2�܂ł̍ŒZ�������ɋA��
        float dist = calcPointLineDist(s1.sp, s2, mp2, t2);
        mp1 = s1.sp;
        t1 = 0.0f;
        return dist;
    }

    //2�������˂���֌W
    float dv1v2 = dot(s1.v, s2.v);
    float dv1v1 = s1.v.magSq();//dot(s1.v,s1v)�Ɠ���
    float dv2v2 = s2.v.magSq();//dot(s2.v,s2v)�Ɠ���
    VECTOR vp2p1 = s1.sp - s2.sp;
    t1 = (dv1v2 * dot(s2.v,vp2p1) - dv2v2 * dot(s1.v,vp2p1))
        / (dv1v1 * dv2v2 - dv1v2 * dv1v2);
    mp1 = s1.sp + s1.v * t1;
    t2 = dot(s2.v, mp1 - s2.sp) / dv2v2;
    mp2 = s2.sp + s2.v * t2;
    return (mp2 - mp1).mag();
}

// 0�`1�̊ԂɃN�����v(�l�������I�ɂ���͈͓��ɂ��邱��)
void clamp0to1(float& v) {
    if (v < 0.0f)
        v = 0.0f;
    else if (v > 1.0f)
        v = 1.0f;
}
// 2�����̍ŒZ����
float calcSegmentSegmentDist
(
    const SEGMENT& s1,//����1
    const SEGMENT& s2,//����2
    VECTOR& mp1, //�����̒[�_1
    VECTOR& mp2, //�����̒[�_2
    float& t1, //�x�N�g���W��1
    float& t2  //�x�N�g���W��2
) {
    float dist = 0;

    /*
    // sv1���k�ނ��Ă���H
    if (sv1.magSq() < 0.000001f) {
        // sv2���k�ށH
        if (sv2.magSq() < 0.000001f) {
            // �_�Ɠ_�̋����̖��ɋA��
            dist = (sp2 - sp1).mag();
            mp1 = sp1;
            mp2 = sp2;
            t1 = t2 = 0.0f;
            segment(mp1, mp2, COLOR(255, 255, 255), 10);
            return dist;
        }
        else {
            // sp1��sv2�̍ŒZ���ɋA��
            dist = calcPointSegmentDist(sp1, sp2, ep2, mp2, t2);
            mp1 = sp1;
            t1 = 0.0f;
            clamp01(t2);
            segment(mp1, mp2, COLOR(255, 255, 255), 10);
            return dist;
        }
    }
    // sv2���k�ނ��Ă���H
    else if (sv2.magSq() < 0.000001f) {
        // sp2��sv1�̍ŒZ���ɋA��
        float dist = calcPointSegmentDist(sp2, sp1, ep1, mp1, t1);
        mp2 = sp2;
        clamp01(t1);
        t2 = 0.0f;
        segment(mp1, mp2, COLOR(255, 255, 255), 10);
        return dist;
    }
    */

    //�Ƃ肠����2�����Ԃ̍ŒZ����,mp1,mp2,t1,t2�����߂Ă݂�
    dist = calcLineLineDist(s1, s2, mp1, mp2, t1, t2);
    if (0.0f <= t1 && t1 <= 1.0f &&
        0.0f <= t2 && t2 <= 1.0f) {
        //mp1,mp2�������Ƃ��������ɂ�����
        segment(mp1, mp2, COLOR(255, 255, 0), 10);
        return dist;
    }

    // �����̒[�_�������̊O�ɂ��鎖�������i���s�ł������ɗ��邱�Ƃ����j----------------
    // t2��0�`1�ɃN�����v����mp2����s1.v�ɐ������~�낵�Ă݂�
    clamp0to1(t2);
    mp2 = s2.sp + s2.v * t2;
    dist = calcPointSegmentDist(mp2, s1, mp1, t1);
    if (0.0f <= t1 && t1 <= 1.0f) {
        //mp1���������ɂ�����
        segment(mp1, mp2, COLOR(255, 0, 0), 10);
        point(mp2, COLOR(255, 0, 0), 40);
        return dist;
    }
    // t1��0�`1�ɃN�����v����mp1����s2.v�ɐ������~�낵�Ă݂�
    clamp0to1(t1);
    mp1 = s1.sp + s1.v * t1;
    dist = calcPointSegmentDist(mp1, s2, mp2, t2);
    if (0.0f <= t2 && t2 <= 1.0f) {
        //mp2���������ɂ�����
        segment(mp1, mp2, COLOR(0, 255, 0), 10);
        point(mp1, COLOR(0, 255, 0), 40);
        return dist;
    }

    // �o���̒[�_���ŒZ�Ɣ���-----------------------------------------------
    //clamp0to1(t1);
    //mp1 = s1.sp + s1.v * t1;
    segment(mp1, mp2, COLOR(0, 255, 255), 10);
    return (mp2 - mp1).mag();
}

void gmain() {
    window(1920, 1080, full);
    hideCursor();
    createSegment();
    createPoint();
    createCapsule();
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
    COLOR capsuleColor = transWhite;
    //�v���W�F�N�V�����s�������Ă���
    createProj();
    //�f���^�^�C��������
    initDeltaTime();
    //���C�����[�v-------------------------------------------------------------
    while (notQuit) {
        //�f���^�^�C���ݒ�
        setDeltaTime();
        //�X�V���̃f�[�^��\�����邽�߁A�����ŃN���A
        clear(180,180,60);
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
            if (dist<0.2f) capsuleColor = transRed;
            else capsuleColor = transWhite;
        }
        //�`��----------------------------------------------------------------
        {
            //��
            if(dispAxisFlag)axis(white, 1);
            //�����P
            s1.draw();
            //�����Q
            s2.draw();
            //�J�v�Z��
            capsule(VECTOR(0,0,0),capsuleColor);

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
