/*---------------------------------------------------------
「ｙ軸に平行な線分」と「平面上の線分」の当たり判定
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

		//線分の移動と描画
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

		//「線分a_b」が 線分c_dを含む「直線」と交わっているか
		VECTOR v1 = a - c;
		VECTOR v2 = b - c;
		VECTOR vn(1, 0);
		float d1 = dot(vn, v1);
		float d2 = dot(vn, v2);
		if (d1 * d2 <= 0)
		{
			//「線分a_b」が「線分c_d」と交わっているか
			float m = Abs(d1);
			float n = Abs(d2);
			//(動画と違いますが、よく考えてみると元の式でだいじょうぶでした)
			VECTOR p = (a * n + b * m) / (m + n);
			if (p.y >= c.y && p.y <= d.y)
			{
				mathPoint(p, 30, COLOR(255, 0, 0));
			}
		}
	}
}
