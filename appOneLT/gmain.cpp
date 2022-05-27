#include"libOne.h"
#include"view_proj.h"
#include"segment.h"
#include"triangle.h"
#include"axis.h"
#include"squareWithHole.h"
#include"point.h"
#include"segment.h"

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
    createSegment();
    createPoint();

    //-----------------------------------------------------------------------
    //�_�̃I���W�i���|�W�V����op
    VECTOR op(0, 0, 0);
    //�_�̍��W�ϊ���̃|�W�V����p
    VECTOR p;
    VECTOR pTran;//�_�̈ړ��p point translate
    VECTOR pRot;//����͎g��Ȃ�
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
    VECTOR triRot(0,0,0.4f);//�O�p�`�̉�]�p triangle rotate

    //-----------------------------------------------------------------------
    //���ʂ��猴�_�܂ł̍ŒZ����
    float d = 0;
    
    //-----------------------------------------------------------------------
    //�_�ƎO�p�`�����W�ϊ����邽�߂̋��p�f�[�^
    float speed = 0.003f;
    //���̑�------------------------------------------------------------------
    //�F
    COLOR red(255, 0, 0);
    COLOR magenta(255, 0, 255);
    COLOR pink(255, 160, 160);
    COLOR grayLight(140, 140, 140);
    COLOR gray(128, 128, 128);
    COLOR white(255, 255, 255);
    COLOR green(0, 255, 0);
    COLOR yellow(255, 255, 60);
    COLOR cyan(0, 255, 255);
    COLOR blue(255, 0, 255);
    //�\���t���b�O
    bool dispAxisFlag = false;
    bool dispNormalFlag = false;
    bool dispSquareFlag = true;
    //�ړ���]������I�u�W�F�N�g�̑I��
    int operateObjSw = 0;
    //�v���W�F�N�V�����s�������Ă���
    createProj();
    //���C�����[�v-------------------------------------------------------------
    while (notQuit) {
        clear();
        //�J�����s����X�V
        updateView();
        //�\���ؑցA����I�u�W�F�N�g�؂�ւ�--------------------------------------
        {
            if (isTrigger(KEY_X)) { dispAxisFlag = !dispAxisFlag; }
            if (isTrigger(KEY_C)) { dispNormalFlag = !dispNormalFlag; }
            if (isTrigger(KEY_V)) { dispSquareFlag = !dispSquareFlag; }
            if (isTrigger(KEY_Z)) { operateObjSw = 1 - operateObjSw; }
            if (isTrigger(KEY_R)) {
                pTran.set(0, 0, 0);
                pRot.set(0, 0, 0);
                triTran.set(0, 0, 0);
                triRot.set(0, 0, 0.2f);
            }
        }
        //�_�𓮂���---------------------------------------------------------
        {
            if (operateObjSw == 0) {
                input(pTran, pRot, speed);
            }
            gWorld.identity();
            gWorld.mulTranslate(pTran);
            gWorld.mulRotateYXZ(pRot);
            p = gWorld * op;
        }
        //�O�p�`�̒��_�𓮂���--------------------------------------------------
        {
            if (operateObjSw == 1) {
                input(triTran, triRot, speed);
            }
            //����������͈ړ����Ă���̉�]������
            gWorld.identity();
            gWorld.mulRotateYXZ(triRot);
            gWorld.mulTranslate(triTran);
            for (int i = 0; i < 3; i++) {
                tp[i] = gWorld * otp[i];
            }
            //�O�p�`�̖@������]������
            gWorld.identity();
            gWorld.mulRotateYXZ(triRot);
            n = gWorld * on;
        }
        //���ʂ̎������߁up.x��p.z�v����up.y�v�����߂�----------------------------
        {
            // ���ʂ̎� ax+by+cz+d=0;
            // �x�N�g��(a,b,c)�͖ʂ̖@���Bd�͕��ʂ��猴�_�܂ł̍ŒZ�����B
            // �����d�����߂�
            // d=-ax-by-cz
            //d = -n.x * tp[0].x - n.y * tp[0].y - n.z * tp[0].z;
            d = dot(-n, tp[0]);
            // y=(-ax-cz-d)/b
            p.y = (-n.x * p.x - n.z * p.z - d) / n.y;
        }
        //�`��----------------------------------------------------------------
        {
            if (dispAxisFlag) {
                //��
                axis(white, 2);
            }
            if(dispNormalFlag){
                //�@���x�N�g��
                segment(VECTOR(0, 0, 0), n, white, 6);
                //���ʂ��猴�_�܂ł̍ŒZ�����̌����鉻
                segment(VECTOR(0, 0, 0), n * -d, yellow, 7);
            }
            if (!dispSquareFlag) {
                //���ʂ̎� ax + by + cz + d = 0 �� d ��
                //�u-n�ƕ��ʏ�̂P�_�Ƃ̓��ρv�Ƃ������Ƃ��킩�����
                segment(VECTOR(0, 0, 0), -n, red, 5);
                segment(VECTOR(0, 0, 0), tp[1], magenta, 5);
            }
            //����
            triangle(tp, grayLight);
            if (dispSquareFlag) {
                squareWithHole(triTran, triRot, gray);
            }
            //�|�C���g
            VECTOR head = p + VECTOR(0, 0.2f, 0);
            segment(p, head, cyan,9);
            point(head, cyan, 30);
            point(p, pink, 20);
            point(VECTOR(p.x,0,p.z), green, 20);
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
            text((let)"���ʂ��猴�_�܂ł̍ŒZ����:"+d, colL, ++num * rowH);
        }
    }
}
