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
	VECTOR osp(0, 0.25f);
	VECTOR oep(0, -0.25f);
	VECTOR sp, ep;
	VECTOR sTran(0.5f,0.5f), sRot;
	//直線と垂線
	VECTOR op0(1, 0);
	VECTOR op1(-1, 0);
	VECTOR ovn(0, 1);
	VECTOR p0, p1, vn;
	VECTOR lTran, lRot;
	//交点の座標
	VECTOR ip;

	float moveSpeed = 0.006f;
	float angSpeed = 0.01f;
	MATRIX world;

	bool dispPointFlag = false;
	int dispSegSw = 0;
	int operateObjSw = 0;

	COLOR white(210, 210, 210);
	COLOR pink(255, 128, 128);
	COLOR green(0, 255, 0);
	COLOR yellow(255, 255, 0);
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
		sp = world * osp;
		ep = world * oep;
		//直線
		if (operateObjSw == 1) {
			input(lTran, lRot, moveSpeed, angSpeed);
		}
		world.translate(lTran);
		world.mulRotateZ(lRot.z);
		p0 = world * op0;
		p1 = world * op1;
		//直線の垂線ベクトル
		world.rotateZ(lRot.z);
		vn = world * ovn;

		//「線分a_b」が 線分c_dを含む「直線」と交わっているか
		VECTOR v1 = sp - lTran;
		VECTOR v2 = ep - lTran;
		float d1 = dot(vn, v1);
		float d2 = dot(vn, v2);
		if (d1 * d2 <= 0){
			//交点の座標（内分点）
			float m = Abs(d1);
			float n = Abs(d2);
			ip = (sp * n + ep * m) / (m + n);
			lineColor = red;
			dispPointFlag = true;
		}
		else {
			lineColor = gray;
			dispPointFlag = false;
		}

		//描画
		mathSegment(p0, p1, lineColor, 10);
		mathSegment(sp, ep, white, 10);
		mathPoint(sp, pink, 20);
		mathPoint(ep, green, 20);
		if (dispPointFlag) {
			mathPoint(ip, yellow, 20);
		}
		//解説用ベクトル描画
		if (isTrigger(KEY_C))++dispSegSw %= 6;
		switch (dispSegSw) {
		case 1:
			mathSegment(lTran, lTran + v1, pink, 3);
			break;
		case 2:
			mathSegment(lTran, lTran + v1, pink, 3);
			mathSegment(lTran, lTran + v2, green, 3);
			break;
		case 3:
			mathSegment(lTran, lTran + v1, pink, 3);
			mathSegment(lTran, lTran + v2, green, 3);
			mathSegment(lTran, lTran + vn, yellow, 3);
			break;
		case 4:
			mathSegment(lTran, lTran + v1, pink, 3);
			mathSegment(lTran, lTran + v2, green, 3);
			mathSegment(lTran, lTran + vn, yellow, 3);
			mathSegment(lTran, lTran + vn * d1, pink, 3);
			mathSegment(lTran + v1, lTran + vn * d1, pink, 3);
			break;
		case 5:
			mathSegment(lTran, lTran + v1, pink, 3);
			mathSegment(lTran, lTran + v2, green, 3);
			mathSegment(lTran, lTran + vn, yellow, 3);
			mathSegment(lTran, lTran + vn * d1, pink, 3);
			mathSegment(lTran + v1, lTran + vn * d1, pink, 3);
			mathSegment(lTran, lTran + vn * d2, green, 3);
			mathSegment(lTran + v2, lTran + vn * d2, green, 3);
		}
		textSize(50);
		text((let)"d1 * d2 = " + d1 * d2, 800, 60);
	}
}
