#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"
#include"mathUtil.h"
#include"QUATERNION.h"

static const int num = 36;
static VECTOR op[num];

void createCircle()
{
    //����������~
    float deg = 360.0f / num;
    for (int i = 0; i < num; i++)
    {
        float rad = deg * i * TO_RAD;
        op[i].set(Cos(rad), 0, -Sin(rad));
    }
}
void circleEu(VECTOR n, const COLOR& c)
{
    VECTOR r(0, 0, 0);
    //�~�̒��_��������]������p�x-----------------------------
    n.normalize();
    //VECTOR(0,1,0)�Ƃ��̓��ς�cos���A���̏ꍇcos��=n.y�ƂȂ�
    r.x = Acos(n.y);
    //�~�̒��_��������]������p�x------------------------------
    n.y = 0;//�Q�����x�N�g�����B�i���A�����ʂōl����j
    n.normalize();
    //��ӂ�dot(VECTOR(0,0,1),n)=n.z;
    //������cross(VECTOR(0,0,1),n)=n.x;
    r.y = Atan2(n.x, n.z);

    MATRIX world;
    world.identity();
    world.mulRotateYXZ(r);
    //�����č��W�ϊ�
    VECTOR p[num];
    for (int i = 0; i < num; i++) {
        p[i] = world * op[i];
        p[i] = gView * p[i];
        p[i] = gProj * p[i];
    }
    //�`��
    for (int i = 0; i < num-2; i++) {
        triangle3D(p[0], p[i + 1], p[i + 2], c, c, c);
    }
}
//�N�H�[�^�j�I���ł������Ɍ����ĕ`��
void circleQt(VECTOR n, const COLOR& c)
{
    //�I���W�i���̉~�̖@���͏�������Ă���
    VECTOR on(0, 1, 0);
    //on��n�Ɍ����邽�߂̉�]��axis�����߂�
    n.normalize();
    VECTOR axis = cross(on, n);
    axis.normalize();
    //��]�p�x�����߂�
    float theta = Acos(dot(on, n));
    //���\��
    text((let)"axis(" + 
        axis.x + "," + 
        axis.y + "," + 
        axis.z + ")" +
        " thata:" + theta, 1300, 35);
    //�N�H�[�^�j�I�����烏�[���h���W�ϊ��s������
    QUATERNION q(axis, theta);
    MATRIX world;
    world.identity();
    world.mulRotateQuaternion(q);
    //�����č��W�ϊ�
    VECTOR p[num];
    for (int i = 0; i < num; i++) {
        p[i] = world * op[i];
        p[i] = gView * p[i];
        p[i] = gProj * p[i];
    }
    //�`��
    for (int i = 0; i < num - 2; i++) {
        triangle3D(p[0], p[i + 1], p[i + 2], c, c, c);
    }
}

