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
    if (pv.magSq() < radiusSq)return true;

    //�����̎n�_���狅�̂̒��S�_�܂ł̃x�N�g��
    pv = p - sp;
    if (pv.magSq() < radiusSq)return true;

    //����(�n�_����I�_��)�̐��K���x�N�g��
    VECTOR sv = (ep - sp).normalize();
    float d = dot(sv, pv);
    {
        //����p�x�N�g���\��
        static int sw = 5;
        if (isTrigger(KEY_C)) { --sw; if (sw < 0)sw = 5; }
        switch (sw) {
        case 0://�ŒZ�x�N�g��
            segment(sp + sv * d, p, COLOR(255, 255, 0), 12);
        case 1://���K���x�N�g��������
            segment(sp, sp + sv * d, COLOR(255, 0, 255), 12);
        case 2:
            textSize(50);
            text((let)"����:" + d,800,100);
        case 3: //�������K���x�N�g��
            segment(sp, sp + sv, COLOR(0, 255, 255), 9);
        case 4: //�n�_�˒��S�_
            segment(sp, p, COLOR(0, 255, 0), 12);
        }
    }
    if (0.0f < d && d < segLen) {
        //�������狅�̂̒��S�_�܂ł̍ŒZ�x�N�g��
        VECTOR nearV = pv - sv * d;
        if (nearV.magSq() < radiusSq) {
            return true;
        }
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
        clear(50,100,200);
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
            bool flag = intersects(p, radius, sp, ep, segLen);
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
            text((let)"", colL, ++num * rowH);
        }
    }
}
