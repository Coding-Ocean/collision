#include"libOne.h"
#include"view_proj.h"
#include"segment.h"
#include"axis.h"
#include"squareWithHole.h"
#include"point.h"
#include"segment.h"
#include"TRIANGLE.h"

void gmain() {
    window(1920, 1080, full);
    hideCursor();
    createSegment();
    createPoint();
    //-----------------------------------------------------------------------
    //�t�B�[���h���\������O�p�|���S���i���W�͌Œ�j
    const int num = 8;
    TRIANGLE triangles[num];
    VECTOR center;
    //�����̎R
    center.set(-1.0f, 0.8f, 0.3f);
    triangles[0].create(center, VECTOR(-2, 0, -1), VECTOR(-2, 0, 1));
    triangles[1].create(center, VECTOR(-2, 0, 1), VECTOR(0, 0, 1));
    triangles[2].create(center, VECTOR(0, 0, 1), VECTOR(0, 0, -1));
    triangles[3].create(center, VECTOR(0, 0, -1), VECTOR(-2, 0, -1));
    //�E���̒J
    center.set(1.3f, -0.5f, -0.5f);
    triangles[4].create(center, VECTOR(0, 0, -1), VECTOR(0, 0, 1));
    triangles[5].create(center, VECTOR(0, 0, 1), VECTOR(2, 0, 1));
    triangles[6].create(center, VECTOR(2, 0, 1), VECTOR(2, 0, -1));
    triangles[7].create(center, VECTOR(2, 0, -1), VECTOR(0, 0, -1));
    //-----------------------------------------------------------------------
    //�L�����N�^(�_)
    VECTOR p;
    float angle = 0;
    int dispCharaSw = 1;
    //���̑�------------------------------------------------------------------
    //�F
    COLOR cyan(0, 255, 255);
    COLOR white(255, 255, 255);
    //�v���W�F�N�V�����s�������Ă���
    createProj();
    //�f���^�^�C��������
    initDeltaTime();
    //���C�����[�v-------------------------------------------------------------
    while (notQuit) {
        //�f���^�^�C���ݒ�
        setDeltaTime();
        //�X�V���̃f�[�^��\�����邽�߁A�����ŃN���A
        clear();
        //�J�����s����X�V
        updateView();
        //�L�����N�^(�_)��ȉ~�^��������----------------------------------------------------
        {
            //p.x��p.z�����ݒ�
            angle += 0.6f * delta;
            p.x = Cos(angle) * 1.8f;
            p.z = Sin(angle) * 0.8f;
            //p.y�͂Ƃ肠�����[���I�I�I�I
            p.y = 0.0f;
        }
        //�O�p�`���_(p.x p.z)���܂�ł�����p.y���擾����--------------------------
        {
            for (int i = 0; i < num; i++) {
                if (triangles[i].contains(p)) {
                    p.y = triangles[i].getY(p);
                    break;
                }
            }
        }
        //�`��----------------------------------------------------------------
        {
            //�t�B�[���h
            for (int i = 0; i < num; i++) {
                triangles[i].draw();
            }
            //�L�����N�^
            if (isTrigger(KEY_C)) {
                ++dispCharaSw %= 2;
            }
            if (dispCharaSw == 0) {
                //�L�����N�^(�_)
                point(p, white, 80);
            }
            else {
                //�L�����N�^(�}�b�`�_)
                VECTOR head = p + VECTOR(0, 0.4f, 0);
                segment(p, head, cyan, 30);
                point(head, cyan, 80);
            }
            //�e�L�X�g���
            float size = 30;
            textSize(size);
            float colL = 10;//��̎n�܂�
            float rowH = size + 10;//�s�̍���
            int num = 0;//�s�ԍ�
            text((let)"p.y:" + p.y, colL, ++num * rowH);
        }
    }
}
