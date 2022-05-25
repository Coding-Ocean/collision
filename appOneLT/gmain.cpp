// �����̎�
#include"libOne.h"

void mathSegment(const VECTOR& s, const VECTOR& e,
	const COLOR& c, float sWeight)
{
	strokeWeight(sWeight);
	stroke(c);
	mathLine(s.x, s.y, e.x, e.y);
}

void mathPoint(const VECTOR& p, const COLOR& c, float sWeight)
{
	strokeWeight(sWeight);
	stroke(c);
	mathPoint(p.x, p.y);
}

void input(VECTOR& t, VECTOR& r, float moveSpeed, float angSpeed)
{
	if (isPress(KEY_SHIFT)) {
		if (isPress(KEY_D)) { r.z += -angSpeed; }
		if (isPress(KEY_A)) { r.z += angSpeed; }
	}
	else {
		if (isPress(KEY_D)) { t.x += moveSpeed; }
		if (isPress(KEY_A)) { t.x += -moveSpeed; }
		if (isPress(KEY_W)) { t.y += moveSpeed; }
		if (isPress(KEY_S)) { t.y += -moveSpeed; }
	}
}

void gmain()
{
	window(1920, 1080, full);
	//�����Ɛ���
	VECTOR osp(-1, -0.5f);
	VECTOR oep= -osp;
	VECTOR on(osp.y,-osp.x);
	on.normalize();
	VECTOR sp, ep, n;
	VECTOR lTran, lRot;
	//�_��
	VECTOR op(0, 0);
	VECTOR p;
	VECTOR pTran, pRot;
	//�����Ɠ_���ړ���]�����鋤�L�f�[�^
	float moveSpeed = 0.006f;
	float angSpeed = 0.01f;
	MATRIX world;
	//�ړ��A��]�����镨�̐؊�(0:�����A1:�_)
	int operateObjSw = 0;
	//�F
	COLOR white(210, 210, 210);
	COLOR pink(255, 128, 128);
	COLOR cyan(0, 255, 255);
	COLOR green(0, 255, 0);
	COLOR yellow(255, 255, 0);
	COLOR lineColor;
	COLOR gray(128, 128, 128);
	COLOR red(255, 0, 0);
	//���C�����[�v
	while (notQuit)
	{
		clear(0, 50, 0);
		mathAxis(2.1f, 255);
		//����Ώې؊�
		if (isTrigger(KEY_Z))operateObjSw = 1 - operateObjSw;
		//�_�̈ړ�----------------------------------------------------
		if (operateObjSw == 1) {
			input(pTran, pRot, moveSpeed, angSpeed);
		}
		world.translate(pTran);
		p = world * op;
		//�����̉�]�A�ړ�----------------------------------------------
		if (operateObjSw == 0) {
			input(lTran, lRot, moveSpeed, angSpeed);
		}
		world.translate(lTran);
		world.mulRotateZ(lRot.z);
		sp = world * osp;
		ep = world * oep;
		//�����̐����x�N�g���̉�]
		world.rotateZ(lRot.z);
		n = world * on;
		//�����̎�������p.x����p.y���v�Z����-----------------------------
		// �����̎��̈�ʌ` 
		// ax + by + c = 0
		// a�͐����x�N�g���̂��Ab�͐����x�N�g���̂��Ac�͒������猴�_�܂ł̍ŒZ����
		// c�ɂ��ĉ�����
		// c = -ax-by
		// ���̃v���O�����̕ϐ������ɓ��Ă͂߂������߂�B
		// x,y���W�͒�����̂ǂ̂P�_�ł��悢�B
		// �����x�N�g���Ɣ��Ε����Ɍ��_�������c<0�ƂȂ�
		float c = -n.x * sp.x - n.y * sp.y;//������ē��ς����dot(-n,sp)
		// �����̎���y�ɂ��ĉ�����
		// y = ( -ax - c ) / b
		// a,b,c���킩���Ă���̂œ_p.x����p.y�����߂邱�Ƃ��ł���
		p.y = (-n.x * p.x - c) / n.y;

		//�`��------------------------------------------------------------
		//����
		mathSegment(sp, ep, lineColor, 6);
		//�����x�N�g��
		mathSegment(VECTOR(0, 0), n, lineColor, 1);
		//c�̌����鉻
		mathSegment(VECTOR(0, 0), n * -c, yellow, 6);
		//�_��
		VECTOR head = p + VECTOR(0, 0.2f);
		mathSegment(p, head, cyan, 10);
		mathPoint(head, pink, 20);

		//�������߂鎮�͋t�����x�N�g����s���Ƃ̓��ςƓ���
		//mathSegment(VECTOR(0, 0), -n*dot(-n,sp), red, 3);
		//mathSegment(VECTOR(0, 0), -n, red, 3);
		//mathSegment(VECTOR(0, 0), sp, red, 3);

		//�e�L�X�g���
		float size = 50;
		float rowH = size + 10;
		float colL = 10;
		int num=0;
		textSize(size);
		if(operateObjSw==0)	
			text((let)"������", colL, ++num*rowH);
		else
			text((let)" �_ ��", colL, ++num * rowH);
		if (isPress(KEY_SHIFT))
			text((let)"��] : shift + AD", colL + size * 3, num * rowH);
		else
			text((let)"�ړ� : ADWS", colL + size * 3, num * rowH);
		text((let)"����Ώې؊� : Z", colL, ++num * rowH);
		text((let)"���_�܂ł̍ŒZ����:" + c, colL, ++num*rowH);
	}
}
