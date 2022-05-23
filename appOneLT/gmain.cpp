//�e�X�g��
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
    //VECTOR osp(0,  0, 0);//original start point
    //�����̍��W�ϊ���̃|�W�V����sp,ep
    //VECTOR sp;//start point
    VECTOR segTran(0.5f,0.3f,0.0f);//�Z�O�����g�̈ړ��p segment translate
    VECTOR segRot;//�Z�O�����g�̉�]�p segment rotate

    //-----------------------------------------------------------------------
    //�O�p�`���_�̃I���W�i���|�W�V����op[3]�Ɩ@���x�N�g��onv
    VECTOR op[2][3];
    VECTOR onv[2];
    //�����v���ɒ��_��p�ӂ���
    float y = -0.1f;
    op[0][0].set(0,y,-1);
    op[0][1].set(-1,0,0);
    op[0][2].set(0,y,1);
    onv[0] = cross(op[0][1] - op[0][0], op[0][2] - op[0][0]);
    onv[0].normalize();

    op[1][0].set(0, y, 1);
    op[1][1].set(1, 0, 0);
    op[1][2].set(0, y, -1);
    onv[1] = cross(op[1][1] - op[1][0], op[1][2] - op[1][0]);
    onv[1].normalize();

    //�O�p�`���_�̍��W�ϊ���̃|�W�V����p[3]�Ɩ@���x�N�g��nv
    VECTOR p[2][3];
    VECTOR nv[2];
    VECTOR triTran;//�O�p�`�̈ړ��p triangle translate
    VECTOR triRot;//�O�p�`�̉�]�p triangle rotate
    //-----------------------------------------------------------------------
    //�����ƎO�p�`�����W�ϊ����邽�߂̋��p�f�[�^
    MATRIX world;
    float speed = 0.005f;
    //-----------------------------------------------------------------------
    //�_����O�p�|���S�����܂ޖ������ʂ܂ł̋���
    float distance = 0;

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
    COLOR triColor[2];//�O�p�`�̐F
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
            //world.identity();
            //world.mulTranslate(segTran);
            //world.mulRotateYXZ(segRot);
            //sp = world * osp;
        }
        //�O�p�`�̒��_�𓮂���--------------------------------------------------
        {
            if (operateObjSw == 1) {
                input(triTran, triRot, speed);
            }
            world.identity();
            world.mulTranslate(triTran);
            world.mulRotateYXZ(triRot);
            for (int j = 0; j < 2; j++) {
                for (int i = 0; i < 3; i++) {
                    p[j][i] = world * op[j][i];
                }
            }
            //�O�p�`�̖@������]������
            world.identity();
            world.mulRotateYXZ(triRot);
            for (int j = 0; j < 2; j++) {
                nv[j] = world * onv[j];
            }
        }
        //�����蔻��----------------------------------------------------------
        {
            bool flag = false;
            for (int j = 0; j < 2; j++) {
                distance = dot(nv[j], segTran - p[j][0]);
                triColor[j] = grayLight;//�������Ă��Ȃ��Ƃ��O���C
                bool containFlag = true;//�Ƃ肠�����܂܂�Ă��邱�Ƃɂ���
                for (int i = 0; i < 3; i++) {
                    VECTOR side = p[j][(i + 1) % 3] - p[j][i];//�O�p�`�̈�ӂ̃x�N�g��
                    VECTOR p_segTran = segTran - p[j][i];//�O�p�`�̂P�̒��_��������_�܂ł̃x�N�g��
                    VECTOR c = cross(side, p_segTran);
                    float d = dot(nv[j], c);
                    if (d < 0) {//�O�σx�N�g�����@���x�N�g���Ƌt�����ɂȂ��Ă���
                        containFlag = false;//ip���O�p�`�Ɋ܂܂�Ă��Ȃ�
                    }
                    //�O�σx�N�g���\��
                    //if (dispCrossFlag) {
                    //    float thickness = 1.2f;
                    //    segment(p[i], p[i] + c, crossColor[i], thickness);//�O�σx�N�g���\��
                    //    segment(p[i], p[(i + 1) % 3], crossColor[i], thickness);//�Ӄx�N�g���\��
                    //    segment(p[i], segTran, crossColor[i], thickness);//��_�܂ł̃x�N�g��
                    //}
                }
                if (containFlag) {
                    flag = true;
                    triColor[j] = red;
                    segTran = segTran + nv[j] * (0.1f - distance);
                }
            }
            if (flag == false) {
                VECTOR v1 = p[0][2] - p[0][0];
                v1.normalize();
                VECTOR v2 = segTran - p[0][0];
                VECTOR v3 = cross(v1, v2);
                float dist = v3.mag();
                if (dist < 0.1f) {
                    v3.normalize();
                    VECTOR v4 = cross(v3, v1);
                    segTran = segTran + v4 * (0.1f - dist);
                }
                text(dist, 900, 100);
            }
        }
        //�`��----------------------------------------------------------------
        {
            if (dispAxisFlag) { 
                axis(white, 0.4f);
            }
            //segment(segTran, segTran+(-nv*distance), white, 1.5f);
            point(segTran, white);
            
            if (dispSquareFlag) { 
                squareWithHole(triTran, triRot, gray);
            }
            
            triangle(p[0], triColor[0]);
            triangle(p[1], triColor[1]);

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
            //text((let)"�O�p�`�̖@��:" + nv.x + " " + nv.y + " " + nv.z, colL, ++num * rowH);
        }
    }
}
