#include"libOne.h"
#include"view_proj.h"
#include"segment.h"
#include"triangle.h"
#include"axis.h"
#include"squareWithHole.h"
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
    //�����̃I���W�i���|�W�V����osp,oep
    VECTOR osp(0,  0.2f, 0);//original start point
    VECTOR oep(0, -0.2f, 0);//original end point
    //�����̍��W�ϊ���̃|�W�V����sp,ep
    VECTOR sp;//start point
    VECTOR ep;//end point
    VECTOR segTran(0.5f,0.3f,0.0f);//�Z�O�����g�̈ړ��p segment translate
    VECTOR segRot;//�Z�O�����g�̉�]�p segment rotate

    //-----------------------------------------------------------------------
    //�O�p�`���_�̃I���W�i���|�W�V����op[3]�Ɩ@���x�N�g��onv
    VECTOR op[3];
    //�����v���ɒ��_��p�ӂ���
    angleMode(DEGREES);
    op[0].set(Sin(   0)*0.5f, 0.0f, -Cos(   0)*0.5f);
    op[1].set(Sin(-120)*0.5f, 0.0f, -Cos(-120)*0.5f);
    op[2].set(Sin(-240)*0.5f, 0.0f, -Cos(-240)*0.5f);
    angleMode(RADIANS);
    VECTOR onv = cross(op[1] - op[0], op[2] - op[0]);
    onv.normalize();
    //�O�p�`���_�̍��W�ϊ���̃|�W�V����p[3]�Ɩ@���x�N�g��nv
    VECTOR p[3];
    VECTOR nv;
    VECTOR triTran;//�O�p�`�̈ړ��p triangle translate
    VECTOR triRot;//�O�p�`�̉�]�p triangle rotate
    //���ʂƐ����̌�_
    VECTOR ip;
    //-----------------------------------------------------------------------
    //�����ƎO�p�`�����W�ϊ����邽�߂̋��p�f�[�^
    MATRIX world;
    float speed = 0.003f;

    //���̑�------------------------------------------------------------------
    //�F
    COLOR red(255, 0, 0, 200);
    COLOR grayLight(150, 150, 150, 230);
    COLOR gray(128, 128, 128, 160);
    COLOR white(255, 255, 255);
    COLOR green(0, 255, 0);
    COLOR yellow(255, 255, 60);
    COLOR cyan(0, 255, 255);
    COLOR blue(255, 0, 255);
    COLOR triColor;//�O�p�`�̐F
    COLOR crossColor[3]  { yellow,green,cyan };

    //�\���t���b�O
    bool dispAxisFlag = false;
    bool dispSquareFlag = false;
    bool dispCrossFlag = false;
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
        {
            if (isTrigger(KEY_C)) { dispCrossFlag = !dispCrossFlag; }
            if (isTrigger(KEY_X)) { dispAxisFlag = !dispAxisFlag; }
            if (isTrigger(KEY_Z)) { operateObjSw = 1 - operateObjSw; }
            if (isTrigger(KEY_R)) {
                segTran.set(0.5f,0.3f,0.0f);
                segRot.set(0, 0, 0);
                triTran.set(0, 0, 0);
                triRot.set(0, 0, 0);
            }
        }
        //�����𓮂���---------------------------------------------------------
        {
            if (operateObjSw == 0) {
                input(segTran, segRot, speed);
            }
            world.identity();
            world.mulTranslate(segTran);
            world.mulRotateYXZ(segRot);
            sp = world * osp;
            ep = world * oep;
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
                p[i] = world * op[i];
            }
            //�O�p�`�̖@������]������
            world.identity();
            world.mulRotateYXZ(triRot);
            nv = world * onv;
        }
        //�����蔻��----------------------------------------------------------
        {
            dispSquareFlag = false;//�O�p�`���܂ޕ��ʂƌ���������t���b�O�𗧂Ă�
            triColor = grayLight;//�������Ă��Ȃ��Ƃ���
            //�u�O�p�`���܂ޕ��ʁv�Ɓu�����v���������Ă��邩
            VECTOR v1 = sp - p[0];
            VECTOR v2 = ep - p[0];
            float d1 = dot(nv, v1);
            float d2 = dot(nv, v2);
            if (d1 * d2 <= 0) {//���u�O�p�`���܂ޕ��ʁv�ƌ������Ă���
                dispSquareFlag = true;
                //���ʂƐ������������Ă���_�̍��Wip
                float m = Abs(d1);
                float n = Abs(d2);
                ip = (sp * n + ep * m) / (m + n);//�����_�̍��Wip
                //ip���O�p�`�Ɋ܂܂�Ă��邩
                bool containFlag = true;//�Ƃ肠�����܂܂�Ă��邱�Ƃɂ���
                for (int i = 0; i < 3; i++) {
                    VECTOR side = p[(i + 1) % 3] - p[i];//�O�p�`�̈�ӂ̃x�N�g��
                    VECTOR p_ip = ip - p[i];//�O�p�`�̂P�̒��_��������_�܂ł̃x�N�g��
                    VECTOR c = cross(side, p_ip);
                    float d = dot(nv, c);
                    if (d < 0) {//�O�σx�N�g�����@���x�N�g���Ƌt�����ɂȂ��Ă���
                        containFlag = false;//ip���O�p�`�Ɋ܂܂�Ă��Ȃ�
                    }
                    //�O�σx�N�g���\��
                    if (dispCrossFlag) {
                        float thickness = 1.2f;
                        segment(p[i], p[i] + c, crossColor[i], thickness);//�O�σx�N�g���\��
                        segment(p[i], p[(i+1)%3], crossColor[i], thickness);//�Ӄx�N�g���\��
                        segment(p[i], ip, crossColor[i], thickness);//��_�܂ł̃x�N�g��
                    }
                }
                if (containFlag) {//�O�p�`�ƌ������Ă���
                    triColor = red;
                }
            }
        }
        //�`��----------------------------------------------------------------
        {
            if (dispAxisFlag) { 
                axis(white, 0.4f);
            }

            segment(sp, ep, white, 1.5f);
            
            if (dispSquareFlag) { 
                point(ip, white);
                squareWithHole(triTran, triRot, gray);
            }
            
            triangle(p, triColor);

            //text info
            float size = 30;
            textSize(size);
            float colL = 10;//��̎n�܂�
            float rowH = size + 10;//�s�̍���
            int num = 0;//�s�ԍ�

            if (operateObjSw == 0)
                text("������", colL, ++num * rowH);
            else 
                text("�O�p��",colL, ++num * rowH);

            if (isPress(KEY_SHIFT))
                text("��] : shift+ADWSQE", colL+size * 3, num * rowH);
            else 
                text("�ړ� : ADWSQE", colL+size*3, num * rowH);
            
            text("����Ώې؊� : Z", colL, ++num * rowH);
            text("�ʒu��]���Z�b�g : R", colL, ++num * rowH);
            text("���\�� : X", colL, ++num * rowH);
            text("�O�ϕ\�� : C", colL, ++num * rowH);
            textSize(20);
            text((let)"�O�p�`�̖@��:" + nv.x + " " + nv.y + " " + nv.z, colL, ++num * rowH);
        }
    }
}
