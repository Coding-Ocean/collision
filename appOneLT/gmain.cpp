#include"libOne.h"
#include"view_proj.h"
#include"segment.h"
#include"triangle.h"
#include"axis.h"
#include"squareWithHole.h"
#include"sphere.h"

void input(VECTOR& tran, VECTOR& rot, float speed)
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
    //�_�̃I���W�i���|�W�V����op
    VECTOR op(0, 0, 0);
    //�_�̍��W�ϊ���̃|�W�V����p
    VECTOR p;
    VECTOR pTran;//�Z�O�����g�̈ړ��p point translate
    VECTOR pRot;//�Z�O�����g�̉�]�p point rotate
    //-----------------------------------------------------------------------
    //�O�p�`���_�̃I���W�i���|�W�V����otp[3]�Ɩ@���x�N�g��on
    VECTOR otp[3];
    //�����v���ɒ��_��p�ӂ���
    angleMode(DEGREES);
    otp[0].set(Sin(0) * 0.5f, 0.0f, -Cos(0) * 0.5f);
    otp[1].set(Sin(-120) * 0.5f, 0.0f, -Cos(-120) * 0.5f);
    otp[2].set(Sin(-240) * 0.5f, 0.0f, -Cos(-240) * 0.5f);
    angleMode(RADIANS);
    VECTOR on = cross(otp[1] - otp[0], otp[2] - otp[0]);
    on.normalize();
    //�O�p�`���_�̍��W�ϊ���̃|�W�V����tp[3]�Ɩ@���x�N�g��n
    VECTOR tp[3];
    VECTOR n;
    VECTOR triTran;//�O�p�`�̈ړ��p triangle translate
    VECTOR triRot;//�O�p�`�̉�]�p triangle rotate
    //-----------------------------------------------------------------------
    //�_�ƎO�p�`�����W�ϊ����邽�߂̋��p�f�[�^
    MATRIX world;
    float speed = 0.003f;

    //���̑�------------------------------------------------------------------
    //�F
    COLOR red(255, 0, 0);
    COLOR pink(255, 200, 200);
    COLOR grayLight(140, 140, 140);
    COLOR gray(128, 128, 128);
    COLOR white(255, 255, 255);
    COLOR green(255, 255, 0);
    COLOR yellow(255, 255, 60);
    COLOR cyan(0, 255, 255);
    COLOR blue(255, 0, 255);
    COLOR triColor;//�O�p�`�̐F
    COLOR crossColor[3]{ yellow,green,cyan };

    //�\���t���b�O
    bool dispAxisFlag = false;
    //�ړ���]������I�u�W�F�N�g�̑I��
    int operateObjSw = 0;
    //�v���W�F�N�V�����s�������Ă���
    createProj();
    createSphere();
    //���C�����[�v-------------------------------------------------------------
    while (notQuit) {
        clear(60);
        //�J�����s����X�V
        updateView();
        //�\���ؑցA����I�u�W�F�N�g�؂�ւ�--------------------------------------
        {
            if (isTrigger(KEY_X)) { dispAxisFlag = !dispAxisFlag; }
            if (isTrigger(KEY_Z)) { operateObjSw = 1 - operateObjSw; }
            if (isTrigger(KEY_R)) {
                pTran.set(0, 0, 0);
                pRot.set(0, 0, 0);
                triTran.set(0, 0, 0);
                triRot.set(0, 0, 0);
            }
        }
        //�_�𓮂���---------------------------------------------------------
        {
            if (operateObjSw == 0) {
                input(pTran, pRot, speed);
            }
            world.identity();
            world.mulTranslate(pTran);
            world.mulRotateYXZ(pRot);
            p = world * op;
        }
        //�O�p�`�̒��_�𓮂���--------------------------------------------------
        {
            if (operateObjSw == 1) {
                input(triTran, triRot, speed);
            }
            world.identity();
            world.mulTranslate(triTran);
            world.mulRotateYXZ(triRot);
            for (int i = 0; i < 3; i++) {
                tp[i] = world * otp[i];
            }
            //�O�p�`�̖@������]������
            world.identity();
            world.mulRotateYXZ(triRot);
            n = world * on;
        }
        //���ʂ̎������߁up.x��p.z�v����up.y�v�����߂�-----------------------------------
        // ���ʂ̎� ax+by+cz+d=0;
        // �x�N�g��(a,b,c)�͖ʂ̖@���Bd�͖ʂ��猴�_�܂ł̍ŒZ�����B
        // �����d�����߂�
        // d=-ax-by-bz
        float d = -n.x * tp[0].x - n.y * tp[0].y - n.z * tp[0].z;
        // y=(-ax-cz-d)/b
        p.y = (-n.x * p.x - n.z * p.z - d) / n.y;
        //�`��----------------------------------------------------------------
        {
            if (dispAxisFlag) {
                //��
                axis(white, 0.4f);
                //�@��
                segment(VECTOR(0, 0, 0), n, white);
                //���_���畽�ʂ܂ł̍ŒZ����
                segment(VECTOR(0, 0, 0), n * -d, yellow);
            }
            //����
            triangle(tp, grayLight);
            squareWithHole(triTran, triRot, gray);
            //�|�C���g
            sphere(p, cyan);
            //�e�L�X�g���
            float size = 30;
            textSize(size);
            float colL = 10;//��̎n�܂�
            float rowH = size + 10;//�s�̍���
            int num = 0;//�s�ԍ�
            if (operateObjSw == 0)
                text(" �_ ��", colL, ++num * rowH);
            else
                text("�O�p��", colL, ++num * rowH);
            if (isPress(KEY_SHIFT))
                text("��] : shift+ADWSQE", colL + size * 3, num * rowH);
            else
                text("�ړ� : ADWSQE", colL + size * 3, num * rowH);
            text("����Ώې؊� : Z", colL, ++num * rowH);
            text("�ʒu��]���Z�b�g : R", colL, ++num * rowH);
            text("���\�� : X", colL, ++num * rowH);
            text((let)"���_���畽�ʂ܂ł̍ŒZ����:"+d, colL, ++num * rowH);
        }
    }
}
