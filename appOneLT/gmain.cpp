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
		if (isPress(KEY_D)) { r.z += angSpeed; }
		if (isPress(KEY_A)) { r.z += -angSpeed; }
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
	window(1000, 1000);
	//線分
	VECTOR oa(0, 2);
	VECTOR ob(0, -2);
	VECTOR a, b;
	VECTOR sTran(0,3), sRot;
	//直線と垂線
	VECTOR oc(6, 0);
	VECTOR od(-6, 0);
	VECTOR ovn(0, 1);
	VECTOR c, d, vn;
	VECTOR lTran, lRot;

	float moveSpeed = 0.03f;
	float angSpeed = 0.005f;
	MATRIX world;

	int operateObjSw = 0;

	axisMode(NODRAW);
	
	while (notQuit)
	{
		clear(0, 50, 0);
		mathAxis(8.1f, 255);
		//操作対象切換
		if (isTrigger(KEY_Z))operateObjSw = 1 - operateObjSw;
		//線分
		if (operateObjSw == 0) {
			input(sTran, sRot, moveSpeed, angSpeed);
		}
		world.translate(sTran);
		world.mulRotateZ(sRot.z);
		a = world * oa;
		b = world * ob;
		mathSegment(a, b, COLOR(255, 255, 255), 10);
		//直線
		if (operateObjSw == 1) {
			input(lTran, lRot, moveSpeed, angSpeed);
		}
		world.translate(lTran);
		world.mulRotateZ(lRot.z);
		c = world * oc;
		d = world * od;
		mathSegment(c, d, COLOR(128, 128, 128), 10);
		//直線の垂線ベクトル
		world.rotateZ(lRot.z);
		vn = world * ovn;

		//「線分a_b」が 線分c_dを含む「直線」と交わっているか
		VECTOR v1 = a - lTran;
		VECTOR v2 = b - lTran;
		float d1 = dot(vn, v1);
		float d2 = dot(vn, v2);
		if (d1 * d2 <= 0)
		{
			//交点の座標（内分点）
			float m = Abs(d1);
			float n = Abs(d2);
			VECTOR p = (a * n + b * m) / (m + n);
			mathPoint(p, COLOR(255, 0, 0), 30 );
		}
		VECTOR ofst(0.03f, 0, 0);
		mathSegment(lTran, lTran + vn + ofst, COLOR(255, 255, 0), 3);
		mathSegment(lTran, lTran + vn*d1, COLOR(255, 200, 200), 3);
		mathSegment(lTran, lTran + v1, COLOR(255, 200, 200), 3);
		mathSegment(lTran-ofst, lTran + vn * d2 -ofst, COLOR(0, 255, 0), 3);
		mathSegment(lTran, lTran + v2, COLOR(0, 255, 0), 3);
	}
}
