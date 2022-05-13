/*---------------------------------------------------------
�u�����ɕ��s�Ȑ����v�Ɓu���ʏ�̐����v�̓����蔻��
---------------------------------------------------------*/
#include"libOne.h"

void mathSegment(const VECTOR& s, const VECTOR& e,
	float sWeight, const COLOR& c)
{
	strokeWeight(sWeight);
	stroke(c);
	mathLine(s.x, s.y, e.x, e.y);
}

void mathPoint(const VECTOR& p, float w, const COLOR& c)
{
	strokeWeight(w);
	stroke(c);
	mathPoint(p.x, p.y);
}

void gmain()
{
	window(1000, 1000);
	VECTOR a(1, 1);
	VECTOR b(4, 3);
	VECTOR c(0, 0);
	VECTOR d(0, 2);
	float speed = 0.03f;
	while (notQuit)
	{
		clear(0, 50, 0);
		mathAxis(5, 255);

		//�����̈ړ��ƕ`��
		if (isPress(KEY_L)) { b.x += speed; }
		if (isPress(KEY_J)) { b.x -= speed; }
		if (isPress(KEY_I)) { b.y += speed; }
		if (isPress(KEY_K)) { b.y -= speed; }
		if (isPress(KEY_D)) { c.x += speed; d.x += speed; }
		if (isPress(KEY_A)) { c.x -= speed; d.x -= speed; }
		if (isPress(KEY_W)) { c.y += speed; d.y += speed; }
		if (isPress(KEY_S)) { c.y -= speed; d.y -= speed; }
		mathSegment(a, b, 10, COLOR(255, 255, 255));
		mathSegment(c, d, 10, COLOR(255, 255, 255));

		//�u����a_b�v�� ����c_d���܂ށu�����v�ƌ�����Ă��邩
		VECTOR v1 = a - c;
		VECTOR v2 = b - c;
		VECTOR vn(1, 0);
		float d1 = dot(vn, v1);
		float d2 = dot(vn, v2);
		if (d1 * d2 <= 0)
		{
			//�u����a_b�v���u����c_d�v�ƌ�����Ă��邩
			float m = Abs(d1);
			float n = Abs(d2);
			//(����ƈႢ�܂����A�悭�l���Ă݂�ƌ��̎��ł������傤�Ԃł���)
			VECTOR p = (a * n + b * m) / (m + n);
			if (p.y >= c.y && p.y <= d.y)
			{
				mathPoint(p, 30, COLOR(255, 0, 0));
			}
		}
	}
}
