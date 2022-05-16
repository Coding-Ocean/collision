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
    //�����̃I���W�i���|�W�V�����n�_osp�A�I�_oep
    VECTOR osp(0,  0.3f, 0);
    VECTOR oep(0, -0.3f, 0);
    //���W�ϊ���̐����̎n�_sp�A�I�_ep
    VECTOR sp;
    VECTOR ep;
    VECTOR segTran(0.0f,0.6f,0);//�x�N�g��a�̈ړ��p
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
 
    //-----------------------------------------------------------------------
    //�����ƎO�p�`�����W�ϊ����邽�߂̋��p�f�[�^
    MATRIX world;
    float speed = 0.006f;

    //���̑�------------------------------------------------------------------
    //�F
    COLOR yellow(255, 255, 120);
    COLOR cyan(0, 255, 255);
    COLOR white(255, 255, 255);
    COLOR pink(255, 100, 100);
    COLOR green(0, 255, 0);
    COLOR squareColor;
    COLOR gray(128, 128, 128, 160);
    COLOR red(255, 0, 0, 160);
    //�\���t���b�O
    bool dispAxisFlag = false;
    bool dispVecFlag = false;
    bool dispDotFlag = false;
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
        if (isTrigger(KEY_C)) { dispVecFlag = !dispVecFlag; }
        if (isTrigger(KEY_V)) { dispDotFlag = !dispDotFlag; }
        if (isTrigger(KEY_Z)) { operateObjSw = 1 - operateObjSw; }
        if (isTrigger(KEY_R)) {
            segTran.set(0.0f, 0.6f, 0);
            segRot = sqTran = sqRot = VECTOR(0, 0, 0);
        }
        //�����𓮂���---------------------------------------------------------
        if (operateObjSw == 0) {
            input(segTran, segRot, speed);
        }
        world.identity();
        world.mulTranslate(segTran);
        world.mulRotateYXZ(segRot);
        sp = world * osp;
        ep = world * oep;
        //���ʂ𓮂���--------------------------------------------------
        if (operateObjSw == 1) {
            input(sqTran, sqRot, speed);
        }
        world.identity();
        world.mulTranslate(sqTran);
        world.mulRotateYXZ(sqRot);
        for (int i = 0; i < 4; i++) {
            p[i] = world * op[i];
        }
        //�@���𓮂���
        world.identity();
        world.mulRotateYXZ(sqRot);
        nv = world * onv;
        //�Փ˔���----------------------------------------------------------------
        VECTOR v1 = sp - p[0];
        VECTOR v2 = ep - p[0];
        float d1 = dot(nv, v1);
        float d2 = dot(nv, v2);
        if (d1 * d2 <= 0) {
            float m = Abs(d1);
            float n = Abs(d2);
            VECTOR ip = (sp * n + ep * m) / (m + n);
            VECTOR ofst(0, 0.02f, 0);
            point(ip, white);
            squareColor = red;
        }
        else {
            squareColor = gray;
        }
        //�`��----------------------------------------------------------------
        if (dispAxisFlag) {
            axis(white, 0.4f);
        }
        segment(sp, ep, cyan, 1.5f);
        point(sp, pink);
        point(ep, green);
        square(p, squareColor);
        if (dispVecFlag) {
            segment(p[0], p[0] + nv, yellow);
            segment(p[0], sp, white);
            segment(p[0], ep, white);
        }
        if (dispDotFlag) {
            if (d1 > 0 && d2 > 0) {
                segment(p[0], (p[0] + nv * d1), pink, 2);
                segment(p[0], (p[0] + nv * d2), green, 2);
            }
            else {
                segment(p[0], (p[0] + nv * d2), green, 2);
                segment(p[0], (p[0] + nv * d1), pink, 2);
            }
        }
        //text info
        float size = 30;
        textSize(size);
        fill(255);
        float rowH = size + 5;//�s�̍���
        int num = 0;//�s�ԍ�

        if (operateObjSw == 0)
            text("������", 0, ++num * rowH);
        else
            text("���ʂ�", 0, ++num * rowH);

        if (isPress(KEY_SHIFT))
            text("��] : shift+ADWSQE", size * 3, num * rowH);
        else
            text("�ړ� : ADWSQE", size * 3, rowH);

        text("����Ώې؊� : Z", 0, ++num * rowH);
        text("�ʒu��]���Z�b�g : R", 0, ++num * rowH);
        text("���\�� : X", 0, ++num * rowH);
        ++num;
        textSize(60);
        //text((let)"���ς�:" + d, size * 22, 65);
    }
}
