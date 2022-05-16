#include"libOne.h"
#include"view_proj.h"
#include"segment.h"
#include"triangle.h"
#include"axis.h"
#include"squareWithHole.h"
#include"circle.h"
#include"point.h"

void input(VECTOR& tran,VECTOR& rot,float speed)
{
    if (isPress(KEY_SHIFT)) {
        //��]
        if (isPress(KEY_D)) { rot.y -= speed; }
        if (isPress(KEY_A)) { rot.y += speed; }
        if (isPress(KEY_W)) { rot.x += speed; }
        if (isPress(KEY_S)) { rot.x -= speed; }
        if (isPress(KEY_Q)) { rot.z += speed; }
        if (isPress(KEY_E)) { rot.z -= speed; }
    }
    else {
        //�ړ�
        if (isPress(KEY_D)) { tran.x += speed; }
        if (isPress(KEY_A)) { tran.x -= speed; }
        if (isPress(KEY_W)) { tran.z -= speed; }
        if (isPress(KEY_S)) { tran.z += speed; }
        if (isPress(KEY_Q)) { tran.y += speed; }
        if (isPress(KEY_E)) { tran.y -= speed; }
    }
}

void gmain() {
    window(1920, 1080, full);
    hideCursor();

    //-----------------------------------------------------------------------
    //�x�N�g��a�̃I���W�i���|�W�V����oa
    VECTOR oa(1.0f, 0, 0);//original vector a
    //�x�N�g��a�̍��W�ϊ���̃|�W�V����a
    VECTOR a;//vector a
    VECTOR aTran;
    VECTOR aRot;
 
    //-----------------------------------------------------------------------
    //�x�N�g��b�̃I���W�i���|�W�V����ob
    VECTOR ob(1.0f, 0, 0);//original vector b
    //�x�N�g��a�̍��W�ϊ���̃|�W�V����a
    VECTOR b;//vector a
    VECTOR bTran;
    VECTOR bRot;

    //-----------------------------------------------------------------------
    //�x�N�g��a,b�����W�ϊ����邽�߂̋��p�f�[�^
    MATRIX world;
    float speed = 0.01f;

    //���̑�------------------------------------------------------------------
    //�F
    COLOR red(255, 0, 0, 200);
    COLOR orange(255, 150, 0, 230);
    COLOR white(255, 255, 255);
    COLOR green(0, 255, 0);
    COLOR yellow(255, 255, 60);
    COLOR cyan(0, 255, 255);
    COLOR gray(128, 128, 128, 128);

    //�\���t���b�O
    bool dispAxisFlag = true;
    bool dispCircleFlag = false;
    //�ړ���]������I�u�W�F�N�g�̑I��
    int operateObjSw = 1;
    //�v���W�F�N�V�����s�������Ă���
    createProj();
    createCircle();
    //���C�����[�v-------------------------------------------------------------
    while (notQuit) {
        clear(0);
        //�J�����s����X�V
        updateView();
        //�\���ؑցA����I�u�W�F�N�g�؂�ւ�--------------------------------------
        {
            if (isTrigger(KEY_X)) { dispAxisFlag = !dispAxisFlag; }
            if (isTrigger(KEY_Z)) { operateObjSw = 1 - operateObjSw; }
            if (isTrigger(KEY_R)) {
                oa.set(1, 0, 0);
                ob.set(1, 0, 0);
                aTran = aRot = bTran = bRot = VECTOR(0, 0, 0);
                dispCircleFlag = false;
            }
            if (isTrigger(KEY_F)) {
                //oa.set(0, 0, -1);
                //ob.set(0, 0, -1);
                aTran = aRot = bTran = bRot = VECTOR(0, 0, 0);
                aRot.y = bRot.y = 3.1415926f / 2;
                dispCircleFlag = false;
            }
            if (isTrigger(KEY_A) || isTrigger(KEY_S) || isTrigger(KEY_D) ||
                isTrigger(KEY_Q) || isTrigger(KEY_W) || isTrigger(KEY_E)) {
                dispCircleFlag = true;
            }
        }
        //�x�N�g��a�𓮂���----------------------------------------------------
        {
            if (operateObjSw == 0) {
                input(aTran, aRot, speed);
            }
            world.identity();
            //world.mulTranslate(aTran);
            world.mulRotateYXZ(aRot);
            a = world * oa;
        }
        //�x�N�g��b�𓮂���----------------------------------------------------
        {
            if (operateObjSw == 1) {
                input(bTran, bRot, speed);
            }
            world.identity();
            //world.mulTranslate(bTran);
            world.mulRotateYXZ(bRot);
            b = world * ob;
        }
        //�O�σx�N�g�������߂�--------------------------------------------------
        VECTOR c = cross(a, b);
        //�`��----------------------------------------------------------------
        if (dispAxisFlag) {
            axis(white, 0.4f);
        }
        float thickness = 2.2f;
        VECTOR o(0, 0, 0);
        segment(o, a, yellow, thickness);//�x�N�g��a�\��
        point(a, yellow);
        segment(o, b, cyan, thickness);//�x�N�g��b�\��
        point(b, cyan);
        segment(o, c, red, thickness);//�O�σx�N�g��c
        point(c, red);
        //�O�σx�N�g���ƒ�������~
        c.normalize();
        float radX = Acos(c.y);
        c.y = 0;
        c.normalize();
        float radY = Atan2(c.x, c.z);
        if (dispCircleFlag) {
            circle(VECTOR(radX, radY, 0), gray);
        }
        //text info
        float size = 30;
        textSize(size);
        float rowH = size + 5;//�s�̍���
        int num = 0;//�s�ԍ�
        if (operateObjSw == 0)
            text("����", 0, ++num * rowH);
        else
            text("����", 0, ++num * rowH);
        if (isPress(KEY_SHIFT))
            text("��] : shift+ADWSQE", size * 2, num * rowH);
        else
            text("�ړ� : ADWSQE", size * 2, rowH);
        text("����Ώې؊� : Z", 0, ++num * rowH);
        text("�ʒu��]���Z�b�g : R", 0, ++num * rowH);
        text("���\�� : X", 0, ++num * rowH);
        text((let)"acos(dot(a,b)):" + Acos(dot(a, b))*TO_DEG, 700, 30);
        text((let)"" + radX*TO_DEG + " " + radY*TO_DEG, 1600, 30);
    }
}
