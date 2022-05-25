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
    createSegment();
    createPoint();

    //-----------------------------------------------------------------------
    //�����̃I���W�i���|�W�V�����n�_osp�A�I�_oep
    VECTOR osp(0,  0.3f, 0);
    VECTOR oep(0, -0.3f, 0);
    //���W�ϊ���̐����̎n�_sp�A�I�_ep
    VECTOR sp;
    VECTOR ep;
    VECTOR segTran(0.6f,0.6f,0);//�x�N�g��a�̈ړ��p
    VECTOR segRot;//�x�N�g��a�̉�]�p

    //-----------------------------------------------------------------------
    //���ʃI���W�i���|�W�V����op
    float l = 0.8f;
    VECTOR op[4] = {
        VECTOR(-l, 0, l),
        VECTOR(l, 0, l),
        VECTOR(l, 0, -l),
        VECTOR(-l, 0, -l),
    };
    //���W�ϊ���̕��ʃ|�W�V����sqp
    VECTOR p[4];
    VECTOR sqTran;//�x�N�g��n�̈ړ��p
    VECTOR sqRot;//�x�N�g��n�̉�]�p
    //�x�N�g�����̃I���W�i���|�W�V����onv
    VECTOR onv(0, 1, 0);//original normal vector
    //���W�ϊ���̃x�N�g��nv
    VECTOR nv;//normal vector
    //��_�̍��W
    VECTOR ip;

    //-----------------------------------------------------------------------
    //�����ƎO�p�`�����W�ϊ����邽�߂̋��p�f�[�^
    float speed = 0.006f;

    //���̑�------------------------------------------------------------------
    //�F
    COLOR yellow(255, 255, 120);
    COLOR cyan(0, 255, 255);
    COLOR white(255, 255, 255);
    COLOR white2(160, 160, 160);
    COLOR pink(255, 100, 100);
    COLOR green(0, 255, 0);
    COLOR squareColor;
    COLOR gray(128, 128, 128, 160);
    COLOR red(255, 0, 0, 160);
    //�\���t���b�O
    bool dispAxisFlag = false;
    bool dispIpFlag = false;
    //�\���X�B�b�`
    int dispSw = 0;
    //�ړ���]������I�u�W�F�N�g�̑I��
    int operateObjSw = 0;
    
    //�v���W�F�N�V�����s�������Ă���
    createProj();

    //���C�����[�v-------------------------------------------------------------
    while (notQuit) {
        clear(60);
        //�J�����s����X�V
        updateView();
        //�\���ؑցA����I�u�W�F�N�g�؂�ւ�--------------------------------------
        if (isTrigger(KEY_X)) { dispAxisFlag = !dispAxisFlag; }
        if (isTrigger(KEY_Z)) { operateObjSw = 1 - operateObjSw; }
        if (isTrigger(KEY_R)) {
            segTran.set(0.6f, 0.6f, 0);
            segRot = sqTran = sqRot = VECTOR(0, 0, 0);
        }
        //�����𓮂���---------------------------------------------------------
        if (operateObjSw == 0) {
            input(segTran, segRot, speed);
        }
        gWorld.identity();
        gWorld.mulTranslate(segTran);
        gWorld.mulRotateYXZ(segRot);
        sp = gWorld * osp;
        ep = gWorld * oep;
        //���ʂ𓮂���--------------------------------------------------
        if (operateObjSw == 1) {
            input(sqTran, sqRot, speed);
        }
        gWorld.identity();
        gWorld.mulTranslate(sqTran);
        gWorld.mulRotateYXZ(sqRot);
        for (int i = 0; i < 4; i++) {
            p[i] = gWorld * op[i];
        }
        //�@������]������B�ړ����Ă͂��߁B
        gWorld.identity();
        gWorld.mulRotateYXZ(sqRot);
        nv = gWorld * onv;
        //�Փ˔���----------------------------------------------------------------
        //�n�_�ƏI�_�����ʂ��͂���ł��邩
        VECTOR v1 = sp - sqTran;
        VECTOR v2 = ep - sqTran;
        float d1 = dot(nv, v1);
        float d2 = dot(nv, v2);
        //d1,d2�̕������Ⴆ�΂͂���ł���
        if (d1 * d2 <= 0) {
            //�����_ip�̍��W�����߂�
            float m = Abs(d1);
            float n = Abs(d2);
            ip = (sp * n + ep * m) / (m + n);
            squareColor = red;
            dispIpFlag = true;
        }
        else {
            squareColor = gray;
            dispIpFlag = false;
        }
        //�`��----------------------------------------------------------------
        if (dispAxisFlag) axis(white, 1);
        segment(sp, ep, cyan, 10);
        point(sp, pink, 30);
        point(ep, green, 30);
        if(dispIpFlag)point(ip, white,30);
        square(p, squareColor);
        //�����p�x�N�g���`��
        VECTOR ofst(0, 0, 0);
        if (isTrigger(KEY_C))++dispSw %= 6;
        switch (dispSw) {
        case 5:
            segment(ep, (sqTran + nv * d2), green, 2);
            segment(sqTran + ofst * 2, (sqTran + nv * d2) + ofst * 2, green, 10);
        case 4:
            segment(sp, (sqTran + nv * d1), pink, 2);
            segment(sqTran + ofst, (sqTran + nv * d1) + ofst, pink, 10);
        case 3:
            segment(sqTran, sqTran + nv, yellow, 4);//�@��
        case 2:
            segment(sqTran, ep, green, 5);//�����ւ̃x�N�g��
        case 1:
            segment(sqTran, sp, pink, 5);//�����ւ̃x�N�g��
        }
        //�e�L�X�g���
        float size = 30;
        textSize(size);
        fill(255);
        float colL = 10;//��̍���
        float rowH = size + 10;//�s�̍���
        int num = 0;//�s�ԍ�
        if (operateObjSw == 0)
            text("������", colL, ++num * rowH);
        else
            text("���ʂ�", colL, ++num * rowH);
        if (isPress(KEY_SHIFT))
            text("��] : shift+ADWSQE", colL+size * 3, num * rowH);
        else
            text("�ړ� : ADWSQE", colL+size * 3, rowH);
        text("����Ώې؊� : Z", colL, ++num * rowH);
        text("�ʒu��]���Z�b�g : R", colL, ++num * rowH);
        text("���\�� : X", colL, ++num * rowH);
        text((let)"d1 * d2 = " + d1 * d2, 800, rowH);
    }
}
