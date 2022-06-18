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
    void update(const VECTOR& tran, const VECTOR& rot,
        const VECTOR& osp, const VECTOR& oep) 
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
    VECTOR& mp, //�_p���璼���ɉ��낵�������̒[�_
    float& t //s.v�̒�����1�Ƃ������́us.sp����mp�܂ł̒����v�̊���
) 
{
    t = 0.0f;
    float dvv = s.v.magSq();//dot(s.v, s.v)�Ɠ���
    if (dvv > 0.0f) {
        t = dot(s.v, p - s.sp) / dvv;
        // ��̎��̐���
        // dot(s.v, p-s.sp) �� |s.v||p-s.sp|cos��
        // dvv��|s.v|�̂Q��
        // ��̌v�Z�ŁAt�� |p-s.sp|cos�� / |s.v|�ƂȂ�B
        // �܂�t�́udot�œ��e����������s.v�̒����v�Ƃ��������ɂȂ�
    }
    mp = s.sp + s.v * t;
    return (p - mp).mag();
}

//�_�Ɛ����̍ŒZ����
float calcPointSegmentDist//�ŒZ����
(
    const VECTOR& p, //�_
    const SEGMENT& s, //����
    VECTOR& mp, //�_p��������܂ł̍ŒZ�����ƂȂ�_(�n�_��I�_�ɂȂ邱�Ƃ�����)
    float& t //s.v�̒�����1�Ƃ������́us.sp����mp�܂ł̒����v�̊���
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
    VECTOR& mp1, //�ŒZ���̒[�_�P
    VECTOR& mp2, //�ŒZ���̒[�_�Q
    float& t1, //s1.v�̒�����1�Ƃ������́us1.sp����mp1�܂ł̒����v�̊���
    float& t2  //s2.v�̒�����1�Ƃ������́us2.sp����mp2�܂ł̒����v�̊���
)
{
    //2���������s
    if (cross(s1.v, s2.v).magSq() < 0.000001f) {
        //����1�̎n�_���璼��2�܂ł̍ŒZ�������ɋA������
        float dist = calcPointLineDist(s1.sp, s2, mp2, t2);
        mp1 = s1.sp;
        t1 = 0.0f;
        return dist;
    }

    //2���������s�łȂ�
    //�݂��ɐ����ɂȂ�悤�ȍŒZ���̒[�_mp1,mp2�����߂�
    //���̏��ŋ��߂Ă��� t1 -> mp1 -> t2 -> mp2
    //�ŏ���t1�����߂鎮�̏ڍׂ͉��̃R�����g�ɂ���
    /*
    �������̍ŒZ���������Ԑ��́A�������ɋ��ʂ̐����ƂȂ�B�B�B���̐����̒[�_mp1,mp2
    mp1 = s1.sp + s1.v * t1
    mp2 = s2.sp + s2.v * t2

    t2�����߂鎮
    t2 = dot(s2.v,mp1 - s2.sp) / dot(s2.v,s2.v)
    �ŏI�I��t1�����߂����̂ŁAt2��t1�ŕ\�����邽��mp1��u��������
    t2 = dot(s2.v, s1.sp + s1.v * t1 - s2.sp) / dot(s2.v, s2.v)
    ���z����t1���v�Z���₷�����Ă���
    t2 = dot(s2.v, s1.sp - s2.sp + s1.v * t1) / dot(s2.v, s2.v)
    t2 = { dot(s2.v, s1.sp - s2.sp) + dot(s2.v, s1.v) * t1 } / dot(s2.v, s2.v)


    ���̎�����t1�𓱂��B�o�J���J�ɕό`���Ă����܂��B
    0 = dot(s1.v,mp1-mp2)
    =dot(s1.v,(s1.sp+s1.v*t1)-(s2.sp+s2.v*t2))
    =dot(s1.v, s1.sp-s2.sp + s1.v*t1 - s2.v*t2)
    ���z����t2���O�ɏo��
    =dot(s1.v, s1.sp-s2.sp) + dot(s1.v,s1.v)*t1 - dot(s1.v,s2.v)*t2 )
    t2������
    =dot(s1.v, s1.sp-s2.sp) + dot(s1.v,s1.v)*t1 - dot(s1.v,s2.v)*{ dot(s2.v, s1.sp - s2.sp) + dot(s2.v, s1.v) * t1 } / dot(s2.v, s2.v)
    ��ԉE�̕���������idot(s2.v, s2.v)���|����j
    =dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp) + dot(s2.v,s2.v)*dot(s1.v,s1.v)*t1 - dot(s1.v,s2.v) * {dot(s2.v, s1.sp - s2.sp) + dot(s2.v, s1.v) * t1}
    ��������������
    =dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp) + dot(s2.v,s2.v)*dot(s1.v,s1.v)*t1 - dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) - dot(s1.v,s2.v) * dot(s2.v,s1.v) * t1
    t1�ł�����
    =dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp)  - dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) + { dot(s2.v,s2.v)*dot(s1.v,s1.v) - dot(s1.v,s2.v) * dot(s2.v,s1.v) }* t1
    �ڍ�
    -{dot(s2.v,s2.v)*dot(s1.v,s1.v) - dot(s1.v,s2.v) * dot(s2.v,s1.v)}*t1 = dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp)  - dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp)
    t1=�ɂ��āA�������Ă���
    t1 = dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp)  - dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) / - {dot(s2.v,s2.v)*dot(s1.v,s1.v) - dot(s1.v,s2.v) * dot(s2.v,s1.v)}
    t1 = -dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp)  + dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) / dot(s2.v,s2.v)*dot(s1.v,s1.v) - dot(s1.v,s2.v) * dot(s2.v,s1.v)
    t1 = dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) -�@dot(s2.v,s2.v) * dot(s1.v, s1.sp-s2.sp)  / dot(s1.v,s1.v) * dot(s2.v,s2.v) - dot(s1.v,s2.v) * dot(s1.v,s2.v)
    */
    float dv1v2 = dot(s1.v, s2.v);
    float dv1v1 = s1.v.magSq();//dot(s1.v,s1.v)�Ɠ���
    float dv2v2 = s2.v.magSq();//dot(s2.v,s2.v)�Ɠ���
    VECTOR vp2p1 = s1.sp - s2.sp;
    t1 = (dv1v2 * dot(s2.v, vp2p1) - dv2v2 * dot(s1.v, vp2p1))
        / (dv1v1 * dv2v2 - dv1v2 * dv1v2);
    mp1 = s1.sp + s1.v * t1;
    t2 = dot(s2.v, mp1 - s2.sp) / dv2v2;
    mp2 = s2.sp + s2.v * t2;
    return (mp2 - mp1).mag();
}

//0�`1�̊ԂɃN�����v(�l�������I�ɂ���͈͓��ɂ��邱��)
void clamp0to1(float& v) {
    if (v < 0.0f)  v = 0.0f;
    else if (v > 1.0f)  v = 1.0f;
}

//2�����Ԃ̍ŒZ����
float calcSegmentSegmentDist
(
    const SEGMENT& s1,//����1
    const SEGMENT& s2,//����2
    VECTOR& mp1, //�ŒZ���̒[�_1(�n�_��I�_�ɂȂ邱�Ƃ�����)
    VECTOR& mp2, //�ŒZ���̒[�_2(�n�_��I�_�ɂȂ邱�Ƃ�����)
    float& t1, //s1.v�̒�����1�Ƃ������́us1.sp����mp1�܂ł̒����v�̊���
    float& t2  //s2.v�̒�����1�Ƃ������́us2.sp����mp2�܂ł̒����v�̊���
)
{
    float dist = 0;

    {
        // s1.v���k�ނ��Ă���H
        if (s1.v.magSq() < 0.000001f) {
            // s2.v���k�ށH
            if (s2.v.magSq() < 0.000001f) {
                // �_�Ɠ_�̋����̖��ɋA��
                dist = (s2.sp - s1.sp).mag();
                mp1 = s1.sp;
                mp2 = s2.sp;
                t1 = t2 = 0.0f;
                segment(mp1, mp2, COLOR(255, 255, 255), 10);
                return dist;
            }
            else {
                // s1.sp��s2.v�̍ŒZ���ɋA��
                dist = calcPointSegmentDist(s1.sp, s2, mp2, t2);
                mp1 = s1.sp;
                t1 = 0.0f;
                clamp0to1(t2);
                segment(mp1, mp2, COLOR(255, 255, 255), 10);
                return dist;
            }
        }
        // s2.v���k�ނ��Ă���H
        else if (s2.v.magSq() < 0.000001f) {
            // s2.sp��sv1�̍ŒZ���ɋA��
            float dist = calcPointSegmentDist(s2.sp, s1, mp1, t1);
            mp2 = s2.sp;
            clamp0to1(t1);
            t2 = 0.0f;
            segment(mp1, mp2, COLOR(255, 255, 255), 10);
            return dist;
        }
    }

    //----------------------------------------------------------------
    //�Ƃ肠����2�����Ԃ̍ŒZ����,mp1,mp2,t1,t2�����߂Ă݂�
    dist = calcLineLineDist(s1, s2, mp1, mp2, t1, t2);
    if (0.0f <= t1 && t1 <= 1.0f &&
        0.0f <= t2 && t2 <= 1.0f) {
        //mp1,mp2�������Ƃ��������ɂ�����
        return dist;
    }
    //mp1,mp2�̗����A�܂��͂ǂ��炩���������ɂȂ������̂Ŏ���


    //----------------------------------------------------------------
    //mp1,t1�����ߒ��� �� t2��0�`1�ɃN�����v����mp2����s1.v�ɐ������~�낵�Ă݂�
    clamp0to1(t2);
    mp2 = s2.sp + s2.v * t2;
    dist = calcPointSegmentDist(mp2, s1, mp1, t1);
    if (0.0f <= t1 && t1 <= 1.0f) {
        //mp1���������ɂ�����
        return dist;
    }
    //mp1���������ɂȂ������̂Ŏ���

    //----------------------------------------------------------------
    //mp2,t2�����ߒ��� �� t1��0�`1�ɃN�����v����mp1����s2.v�ɐ������~�낵�Ă݂�
    clamp0to1(t1);
    mp1 = s1.sp + s1.v * t1;
    dist = calcPointSegmentDist(mp1, s2, mp2, t2);
    if (0.0f <= t2 && t2 <= 1.0f) {
        //mp2���������ɂ�����
        return dist;
    }
    //mp2���������ɂȂ������̂Ŏ���

    //----------------------------------------------------------------
    //�����܂ŗ�����Amp1����mp2�܂ł��ŒZ
    return (mp2 - mp1).mag();
}

//�⏕���o�[�W����
COLOR Yellow(255, 255, 0);
COLOR Red(255, 0, 0);
COLOR Green(0, 255, 0);
COLOR Cyan(0, 255, 255);
float calcSegmentSegmentDist_
(
    const SEGMENT& s1,//����1
    const SEGMENT& s2,//����2
    VECTOR& mp1, //�ŒZ���̒[�_1(�n�_��I�_�ɂȂ邱�Ƃ�����)
    VECTOR& mp2, //�ŒZ���̒[�_2(�n�_��I�_�ɂȂ邱�Ƃ�����)
    float& t1, //s1.v�̒�����1�Ƃ������́us1.sp����mp1�܂ł̒����v�̊���
    float& t2  //s2.v�̒�����1�Ƃ������́us2.sp����mp2�܂ł̒����v�̊���
)
{
    float dist = 0;

    //------------------------------------------------------------------------
    //check1:�Ƃ肠����2�����Ԃ̍ŒZ����,mp1,mp2,t1,t2�����߂Ă݂�
    dist = calcLineLineDist(s1, s2, mp1, mp2, t1, t2);
    fill(Yellow);
    if (0.0f <= t1 && t1 <= 1.0f &&
        0.0f <= t2 && t2 <= 1.0f) {
        //mp1,mp2�������Ƃ��������ɂ�����
        print((let)"check1�Z�Ft1="+t1+" t2="+t2);
        point(mp1, Yellow, 30);
        point(mp2, Yellow, 30);
        segment(mp1, mp2, Yellow, 10);
        return dist;
    }
    print((let)"check1�~�Ft1="+t1+" t2="+t2);
    segment(mp1, mp2, Yellow, 2);
    point(mp1, Yellow, 12);
    point(mp2, Yellow, 12);
    //mp1,mp2�̗����A�܂��͂ǂ��炩���������ɂȂ������̂Ŏ���

    //------------------------------------------------------------------------
    //check2:mp1,t1�����ߒ��� �� t2��0�`1�ɃN�����v����mp2����s1.v�ɐ������~�낵�Ă݂�
    clamp0to1(t2);
    mp2 = s2.sp + s2.v * t2;
    dist = calcPointSegmentDist(mp2, s1, mp1, t1);
    fill(Red);
    if (0.0f <= t1 && t1 <= 1.0f) {
        //mp1���������ɂ�����
        print((let)"check2�Z�Ft1=" + t1 + " t2=" + t2);
        point(mp1, Red, 30);
        segment(mp1, mp2, Red, 10);
        return dist;
    }
    print((let)"check2�~�Ft1=" + t1 + " t2=" + t2);
    segment(s1.sp+s1.v*t1, mp2, Red, 2);
    point(s1.sp + s1.v * t1, Red, 12);
    //mp1���������ɂȂ������̂Ŏ���

    //------------------------------------------------------------------------
    //check3:mp2,t2�����ߒ��� �� t1��0�`1�ɃN�����v����mp1����s2.v�ɐ������~�낵�Ă݂�
    clamp0to1(t1);
    mp1 = s1.sp + s1.v * t1;
    dist = calcPointSegmentDist(mp1, s2, mp2, t2);
    fill(Green);
    if (0.0f <= t2 && t2 <= 1.0f) {
        //mp2���������ɂ�����
        print((let)"check3�Z�Ft1=" + t1 + " t2=" + t2);
        segment(mp1, mp2, Green, 10);
        point(mp2, Green, 30);
        return dist;
    }
    print((let)"check3�~�Ft1=" + t1 + " t2=" + t2);
    segment(mp1, s2.sp + s2.v * t2, Green, 2);
    point(s2.sp + s2.v * t2, Green, 12);
    //mp2���������ɂȂ������̂Ŏ���

    //-----------------------------------------------------------------------
    //�����܂ŗ�����Amp1����mp2�܂ł��ŒZ
    fill(Cyan);
    clamp0to1(t2);
    print((let)"last  �@�Ft1=" + t1 + " t2=" + t2);
    segment(mp1, mp2, COLOR(Cyan), 10);
    return (mp2 - mp1).mag();
}

void gmain() {
    window(1920, 1080, full);
    hideCursor();
    createSegment();
    createPoint();
    createCapsule();
    //-----------------------------------------------------------------------
    //�J�v�Z��1���a
    float radius1 = 0.2f;
    //�����P�̃I���W�i���|�W�V����
    VECTOR osp1(0, 0.2f, 0);//original start point
    VECTOR oep1(0, -osp1.y, 0);//original end point
    //�����P���W�ϊ��p�f�[�^
    VECTOR seg1Tran(0.0f, 0.0f, 0.0f);//�Z�O�����g�̈ړ��p segment translate
    VECTOR seg1Rot;//�Z�O�����g�̉�]�p segment rotate
    //�����P(���p�ϊ���̒l������ɃZ�b�g����)
    SEGMENT s1;
    //-----------------------------------------------------------------------
    //�J�v�Z��2���a
    float radius2 = 0.2f;
    //����2�̃I���W�i���|�W�V����
    VECTOR osp2(0, 0.2f, 0);//original start point
    VECTOR oep2(0, -osp2.y, 0);//original end point
    //�����P���W�ϊ��p�f�[�^
    VECTOR seg2Tran(0.5f, 0.0f, 0.0f);//�Z�O�����g�̈ړ��p segment translate
    VECTOR seg2Rot;//�Z�O�����g�̉�]�p segment rotate
    //�����P(���p�ϊ���̒l������ɃZ�b�g����)
    SEGMENT s2;
    //-----------------------------------------------------------
    //�ړ���]�X�s�[�h
    float speed = 0.3f;
    //�\���t���b�O
    bool dispAxisFlag = false;
    //�ړ���]������I�u�W�F�N�g�̑I��
    int operateObjSw = 0;
    //���̑�------------------------------------------------------------------
    //�F
    COLOR yellow(255, 255, 0);
    COLOR white(255, 255, 255);
    COLOR colColor(255, 0, 0, 130);
    COLOR noColColor(255, 255, 255, 130);
    COLOR capsuleColor = noColColor;
    //�v���W�F�N�V�����s�������Ă���
    createProj();
    //�f���^�^�C��������
    initDeltaTime();
    //���C�����[�v-------------------------------------------------------------
    while (notQuit) {
        //�f���^�^�C���ݒ�
        setDeltaTime();
        //�X�V���̃f�[�^��\�����邽�߁A�����ŃN���A
        clear(60,120,240);
        //�J�����s����X�V
        updateView();
        //�\���ؑցA����I�u�W�F�N�g�؂�ւ��A�g�����X�t�H�[�����Z�b�g----------------
        {
            if (isTrigger(KEY_X)) { dispAxisFlag = !dispAxisFlag; }
            if (isTrigger(KEY_Z)) { operateObjSw = 1 - operateObjSw; }
            if (isTrigger(KEY_R)) {
                seg1Tran.set(0, 0, 0); seg1Rot.set(0, 0, 0);
                seg2Tran.set(0.5f, 0.0f, 0.0f); seg2Rot.set(0, 0, 0);
            }
        }
        //����1�𓮂���-----------------------------------------------------------
        {
            if (operateObjSw == 1) {
                input(seg1Tran, seg1Rot, speed*delta);
            }
            s1.update(seg1Tran, seg1Rot, osp1, oep1);
        }
        //����2�𓮂���---------------------------------------------------------
        {
            if (operateObjSw == 0) {
                input(seg2Tran, seg2Rot, speed*delta);
            }
            s2.update(seg2Tran, seg2Rot, osp2, oep2);
        }
        //�����蔻��----------------------------------------------------------
        {
            VECTOR mp1;
            VECTOR mp2;
            float t1;
            float t2;
            float dist = 0;
            {
                //�X�e�b�v���Ƃ̉���p
                //dist = calcPointLineDist(s2.sp, s1, mp1, t1);
                //point(s2.sp, yellow, 40);
                //segment(s2.sp, mp1, yellow, 10);
                //segment(s1.sp,s1.ep, white, 10);
                //print((let)"t="+t1);
                //segment(s1.sp - s1.v * 5, s1.ep + s1.v * 5, white, 10);

                //dist = calcPointSegmentDist(s2.sp, s1, mp1, t1);
                //print((let)"t="+t1);
                //segment(s1.sp+s1.v*t1, s2.sp, yellow, 5);
                //point(s2.sp, yellow, 40);
                //segment(s2.sp, mp1, yellow, 10);
                //segment(s1.sp,s1.ep, white, 10);

                //dist = calcLineLineDist(s1, s2, mp1, mp2, t1, t2);
                //segment(mp2, mp1, yellow, 10);
                //point(mp1, yellow, 40);
                //point(mp2, yellow, 40);
                //segment(s1.sp - s1.v * 5, s1.sp + s1.v * 5, white, 10);
                //segment(s2.sp - s2.v * 5, s2.sp + s2.v * 5, white, 10);
            }
            dist = calcSegmentSegmentDist_(s1, s2, mp1, mp2, t1, t2);
            print((let)"distance="+dist);
            if (dist < radius1+radius2) capsuleColor = colColor;
            else capsuleColor = noColColor;
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
            capsule(seg2Tran, seg2Rot, capsuleColor, osp2.y, radius2);
            capsule(seg1Tran, seg1Rot, capsuleColor, osp1.y, radius1);

            //�e�L�X�g���
            //float size = 30;
            //textSize(size);
            //float colL = 10;//��̎n�܂�
            //float rowH = size + 10;//�s�̍���
            //int num = 0;//�s�ԍ�
            //fill(255);
            //text((let)"", colL, ++num * rowH);
        }
    }
}
