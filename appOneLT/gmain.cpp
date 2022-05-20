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
	window(1920, 1080, full);
	//線分
	VECTOR oa(0, 0.25f);
	VECTOR ob(0, -0.25f);
	VECTOR a, b;
	VECTOR sTran(0.5f,0.5f), sRot;
	//直線と垂線
	VECTOR oc(1, 0);
	VECTOR od(-1, 0);
	VECTOR ovn(0, 1);
	VECTOR c, d, vn;
	VECTOR lTran, lRot;
	//交点の座標
	VECTOR ip;

	float moveSpeed = 0.006f;
	float angSpeed = 0.01f;
	MATRIX world;

	bool dispPointFlag = false;
	int dispSegSw = 0;
	int operateObjSw = 0;

	COLOR lineColor;
	COLOR gray(128, 128, 128);
	COLOR red(255,0,0);

	axisMode(NODRAW);
	
	while (notQuit)
	{
		clear(0, 50, 0);
		mathAxis(2.1f, 255);
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
		//直線
		if (operateObjSw == 1) {
			input(lTran, lRot, moveSpeed, angSpeed);
		}
		world.translate(lTran);
		world.mulRotateZ(lRot.z);
		c = world * oc;
		d = world * od;
		//直線の垂線ベクトル
		world.rotateZ(lRot.z);
		vn = world * ovn;

		//「線分a_b」が 線分c_dを含む「直線」と交わっているか
		VECTOR v1 = a - lTran;
		VECTOR v2 = b - lTran;
		float d1 = dot(vn, v1);
		float d2 = dot(vn, v2);
		if (d1 * d2 <= 0){
			//交点の座標（内分点）
			float m = Abs(d1);
			float n = Abs(d2);
			ip = (a * n + b * m) / (m + n);
			lineColor = red;
			dispPointFlag = true;
		}
		else {
			lineColor = gray;
			dispPointFlag = false;
		}
		//描画
		mathSegment(c, d, lineColor, 10);
		mathSegment(a, b, COLOR(220, 220, 220), 10);
		mathPoint(a, COLOR(255, 180, 180), 20);
		mathPoint(b, COLOR(0, 255, 0), 20);
		if (dispPointFlag) {
			mathPoint(ip, COLOR(255, 255, 0), 20);
		}
		//解説用ベクトル描画
		if (isTrigger(KEY_C))++dispSegSw %= 6;
		switch (dispSegSw) {
		case 1:
			mathSegment(lTran, lTran + v1, COLOR(255, 200, 200), 3);
			break;
		case 2:
			mathSegment(lTran, lTran + v1, COLOR(255, 200, 200), 3);
			mathSegment(lTran, lTran + v2, COLOR(0, 255, 0), 3);
			break;
		case 3:
			mathSegment(lTran, lTran + v1, COLOR(255, 200, 200), 3);
			mathSegment(lTran, lTran + v2, COLOR(0, 255, 0), 3);
			mathSegment(lTran, lTran + vn, COLOR(255, 255, 0), 3);
			break;
		case 4:
			mathSegment(lTran, lTran + v1, COLOR(255, 200, 200), 3);
			mathSegment(lTran, lTran + v2, COLOR(0, 255, 0), 3);
			mathSegment(lTran, lTran + vn, COLOR(255, 255, 0), 3);
			mathSegment(lTran, lTran + vn * d1, COLOR(255, 200, 200), 3);
			mathSegment(lTran + v1, lTran + vn * d1, COLOR(255, 200, 200), 3);
			break;
		case 5:
			mathSegment(lTran, lTran + v1, COLOR(255, 200, 200), 3);
			mathSegment(lTran, lTran + v2, COLOR(0, 255, 0), 3);
			mathSegment(lTran, lTran + vn, COLOR(255, 255, 0), 3);
			mathSegment(lTran, lTran + vn * d1, COLOR(255, 200, 200), 3);
			mathSegment(lTran + v1, lTran + vn * d1, COLOR(255, 200, 200), 3);
			mathSegment(lTran, lTran + vn * d2, COLOR(0, 255, 0), 3);
			mathSegment(lTran + v2, lTran + vn * d2, COLOR(0, 255, 0), 3);
		}
		textSize(50);
		text((let)"d1 * d2 = " + d1 * d2, 800, 60);
	}
}
