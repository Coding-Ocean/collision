#include"libOne.h"
#include"input_.h"
#include"view_proj.h"
#include"axis.h"
#include"segment.h"
#include"point.h"
#include"sphere.h"
bool intersects(
    const VECTOR& p, float radius, //��
    const VECTOR& sp, const VECTOR& ep, float segLen //����
) 
{
    float radiusSq = radius * radius;
    VECTOR pv;

    //�����̏I�_���狅�̂̒��S�_�܂ł̃x�N�g��
    pv = p - ep;
    if (pv.magSq() < radiusSq) return true;

    //�����̎n�_���狅�̂̒��S�_�܂ł̃x�N�g��
    pv = p - sp;
    if (pv.magSq() < radiusSq) return true;

    //����(�n�_����I�_��)�̐��K���x�N�g��
    VECTOR sv = (ep - sp).normalize();
    //sv��pv�̓��ρB�ipv�͎n�_���璆�S�_�܂ł̃x�N�g���j
    float d = dot(sv, pv);
    //���ς̒l���O���傫���A�����x�N�g���̑傫����菬�����Ȃ�
    if (0 < d && d < segLen) {
        //sv�𒷂�d�̃x�N�g���ɂ���
        sv *= d;
        //�x�N�g���̈����Z��pv���u�������狅�̂̒��S�_�܂ł̍ŒZ�x�N�g���v�ɏ���������
        pv -= sv;
        if (pv.magSq() < radiusSq) return true;
    }

    return false;
}
bool intersectsV2(
    const VECTOR& p, float radius, //��
    const VECTOR& sp, const VECTOR& ep, float segLen //����
)
{
    VECTOR sv = ep - sp;
    /*
    �Q�_sp,ep��ʂ钼���ɓ_p���琂�������������̓_mp�����߂�B
    sv�̒������P�Ƃ������́usp����mp�܂ł̒����̊����v��t�Ƃ���B
    mp = sp + sv * t�E�E�Et=0�̎�mp=sp�At=1�̎�mp=ep�ɂȂ�
    t�����߂邽�߂̐��w(���ς̒萔�{�A���z�@��)
    dot(sv, p - mp) = 0
    dot(sv, p - (sp + sv*t) ) = 0
    dot(sv, p - sp - sv*t) = 0
    dot(sv, p - sp) + dot(sv, -sv*t) = 0
    dot(sv, -sv*t) = -dot(sv, p - sp)
    t * dot(sv, -sv) = -dot(sv, p - sp)
    t = -dot(sv, p - sp) / dot(sv, -sv)
    t = -dot(sv, p - sp) / -dot(sv, sv)
    t = dot(sv, p - sp) / dot(sv, sv)
    t = dot(sv, p - sp) / |sv||sv|
    */
    //segLen�̓[���ɂȂ�Ȃ����̂Ƃ���
    float t = dot(sv, p - sp) / (segLen * segLen);
    VECTOR mp;
    if (0.0f < t && t < 1.0f) {
        mp = sp + sv * t;
    }
    else if (t <= 0.0f) {
        mp = sp;
    }
    else {
        mp = ep;
    }

    print((let)"t:"+t);
    segment(mp, p, COLOR(255, 255, 0), 10);
    //segment(sp + sv * t, p, COLOR(255, 0, 0), 10);
    
    if ((p - mp).magSq() < radius * radius) {
        return true;
    }
    return false;
}

void gmain() {
    window(1920, 1080, full);
    hideCursor();
    createSegment();
    createPoint();
    createSphere();
    //-----------------------------------------------------------------------
    //�����̃I���W�i���|�W�V����osp,oep
    VECTOR osp(0, 0.4f, 0);//original start point
    VECTOR oep(0, -0.4f, 0);//original end point
    VECTOR w = oep - osp;
    float segLen = w.mag();
    //�����̍��W�ϊ���̃|�W�V����sp,ep
    VECTOR sp;//start point
    VECTOR ep;//end point
    VECTOR segTran(0.5f, 0.0f, 0.0f);//�Z�O�����g�̈ړ��p segment translate
    VECTOR segRot;//�Z�O�����g�̉�]�p segment rotate
    //-----------------------------------------------------------------------
    //���̂̃I���W�i���|�W�V����op
    VECTOR op(0, 0, 0);
    //���̂̍��p�ϊ���̃|�W�V����p
    VECTOR p;
    VECTOR sphTran(0.0f, 0.0f, 0.0f);//���̂̈ړ��p sphere translate
    VECTOR sphRot;
    float radius = 0.3f;
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
    COLOR sphColor = transWhite;
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
                segTran.set(0.5f, 0.0f, 0.0f);
                segRot.set(0, 0, 0);
                sphTran.set(0, 0, 0);
                sphRot.set(0, 0, 0);
            }
        }
        //�����𓮂���---------------------------------------------------------
        {
            if (operateObjSw == 0) {
                input(segTran, segRot, speed*delta);
            }
            world.identity();
            world.mulTranslate(segTran);
            world.mulRotateYXZ(segRot);
            sp = world * osp;
            ep = world * oep;
        }
        //���𓮂���-----------------------------------------------------------
        {
            if (operateObjSw == 1) {
                input(sphTran, sphRot, speed*delta);
            }
            world.identity();
            world.mulTranslate(sphTran);
            //world.mulRotateYXZ(sphRot);
            p = world * op;
        }
        //�����蔻��----------------------------------------------------------
        {
            bool flag = intersectsV2(p, radius, sp, ep, segLen);
            if (flag) sphColor = transRed;
            else sphColor = transWhite;
        }
        //�`��----------------------------------------------------------------
        {
            //��
            if(dispAxisFlag)axis(white, 1);
            //����
            segment(sp, ep, white, 10);
            point(sp, white, 10);
            point(ep, white, 10);
            //����
            sphere(p, sphColor, radius);
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
