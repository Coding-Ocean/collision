#include"libOne.h"
#include"view_proj.h"
#include"segment.h"
#include"triangle.h"
#include"axis.h"
#include"square.h"
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
    VECTOR oa(1, 0, 0);
    //���W�ϊ���̃x�N�g��a
    VECTOR a;
    VECTOR aTran;//�x�N�g��a�̈ړ��p
    VECTOR aRot;//�x�N�g��a�̉�]�p

    //-----------------------------------------------------------------------
    //�x�N�g�����̃I���W�i���|�W�V����on
    VECTOR on(0, 1, 0);
    //���W�ϊ���̃x�N�g��n
    VECTOR n;//normal
    VECTOR nTran;//�x�N�g��n�̈ړ��p
    VECTOR nRot;//�x�N�g��n�̉�]�p
    //�����p���ʃI���W�i���|�W�V����op
    float l = 1.2f;
    VECTOR op[4] = {
        VECTOR(-l, 0, -l),
        VECTOR(-l, 0, l),
        VECTOR(l, 0, -l),
        VECTOR(l, 0, l),
    };
    //���W�ϊ���̕��ʃ|�W�V����sqp
    VECTOR p[4];
 
    //-----------------------------------------------------------------------
    //�����ƎO�p�`�����W�ϊ����邽�߂̋��p�f�[�^
    MATRIX world;
    float speed = 0.006f;

    //���̑�------------------------------------------------------------------
    //�F
    COLOR yellow(255, 255, 120);
    COLOR cyan(0, 255, 255);
    COLOR red(255, 0, 0);
    COLOR white(255, 255, 255);
    COLOR gray(128, 128, 128, 190);
    //�\���t���b�O
    bool dispAxisFlag = false;
    //�ړ���]������I�u�W�F�N�g�̑I��
    int operateObjSw = 0;
    
    //�v���W�F�N�V�����s�������Ă���
    createProj();

    //���C�����[�v-------------------------------------------------------------
    while (notQuit) {
        clear(0);
        //�J�����s����X�V
        updateView();
        //�\���ؑցA����I�u�W�F�N�g�؂�ւ�--------------------------------------
        if (isTrigger(KEY_X)) { dispAxisFlag = !dispAxisFlag; }
        if (isTrigger(KEY_Z)) { operateObjSw = 1 - operateObjSw; }
        if (isTrigger(KEY_R)) {
            aTran = aRot = nTran = nRot = VECTOR(0, 0, 0);
        }
        //�x�N�g��a�𓮂���---------------------------------------------------------
        if (operateObjSw == 0) {
            input(aTran, aRot, speed);
        }
        world.identity();
        //world.mulTranslate(aTran);
        world.mulRotateYXZ(aRot);
        a = world * oa;
        //�x�N�g��n�𓮂���--------------------------------------------------
        if (operateObjSw == 1) {
            input(nTran, nRot, speed);
        }
        world.identity();
        //world.mulTranslate(nTran);
        world.mulRotateYXZ(nRot);
        n = world * on;
        //�����p���ʂ𓮂���
        for (int i = 0; i < 4; i++) {
            p[i] = world * op[i];
        }
        //���ς�----------------------------------------------------------------
        float d = dot(n, a);
        //�`��----------------------------------------------------------------
        if (dispAxisFlag) {
            axis(white, 0.4f);
        }
        VECTOR o(0, 0, 0);
        segment(o, n, yellow, 1.5f);
        segment(o, a, cyan, 1.5f);
        point(a, cyan);
        segment(o, n * d, red, 3.0f);
        segment(a, n * d, white, 1.0f);
        square(p, gray);

        //text info
        float size = 30;
        textSize(size);
        fill(255);
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
        ++num;
        textSize(60);
        text((let)"���ς�:" + d, size * 22, 65);
        text((let)"acos(d):" + Acos(d)*TO_DEG, size * 44, 65);
    }
}
