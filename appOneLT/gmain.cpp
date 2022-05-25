// 直線の式
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
	//直線と垂線
	VECTOR osp(-1, -0.5f);
	VECTOR oep= -osp;
	VECTOR on(osp.y,-osp.x);
	on.normalize();
	VECTOR sp, ep, n;
	VECTOR lTran, lRot;
	//点ｐ
	VECTOR op(0, 0);
	VECTOR p;
	VECTOR pTran, pRot;
	//直線と点を移動回転させる共有データ
	float moveSpeed = 0.006f;
	float angSpeed = 0.01f;
	MATRIX world;
	//移動、回転させる物の切換(0:直線、1:点)
	int operateObjSw = 0;
	//色
	COLOR white(210, 210, 210);
	COLOR pink(255, 128, 128);
	COLOR cyan(0, 255, 255);
	COLOR green(0, 255, 0);
	COLOR yellow(255, 255, 0);
	COLOR lineColor;
	COLOR gray(128, 128, 128);
	COLOR red(255, 0, 0);
	//メインループ
	while (notQuit)
	{
		clear(0, 50, 0);
		mathAxis(2.1f, 255);
		//操作対象切換
		if (isTrigger(KEY_Z))operateObjSw = 1 - operateObjSw;
		//点の移動----------------------------------------------------
		if (operateObjSw == 1) {
			input(pTran, pRot, moveSpeed, angSpeed);
		}
		world.translate(pTran);
		p = world * op;
		//直線の回転、移動----------------------------------------------
		if (operateObjSw == 0) {
			input(lTran, lRot, moveSpeed, angSpeed);
		}
		world.translate(lTran);
		world.mulRotateZ(lRot.z);
		sp = world * osp;
		ep = world * oep;
		//直線の垂線ベクトルの回転
		world.rotateZ(lRot.z);
		n = world * on;
		//直線の式を求めp.xからp.yを計算する-----------------------------
		// 直線の式の一般形 
		// ax + by + c = 0
		// aは垂直ベクトルのｘ、bは垂直ベクトルのｙ、cは直線から原点までの最短距離
		// cについて解くと
		// c = -ax-by
		// このプログラムの変数を式に当てはめｃを求める。
		// x,y座標は直線上のどの１点でもよい。
		// 垂直ベクトルと反対方向に原点があるとc<0となる
		float c = -n.x * sp.x - n.y * sp.y;//これって内積だよねdot(-n,sp)
		// 直線の式をyについて解くと
		// y = ( -ax - c ) / b
		// a,b,cがわかっているので点p.xからp.yを求めることができる
		p.y = (-n.x * p.x - c) / n.y;

		//描画------------------------------------------------------------
		//直線
		mathSegment(sp, ep, lineColor, 6);
		//垂線ベクトル
		mathSegment(VECTOR(0, 0), n, lineColor, 1);
		//cの見える化
		mathSegment(VECTOR(0, 0), n * -c, yellow, 6);
		//点ｐ
		VECTOR head = p + VECTOR(0, 0.2f);
		mathSegment(p, head, cyan, 10);
		mathPoint(head, pink, 20);

		//ｃを求める式は逆垂線ベクトルとsｐとの内積と同じ
		//mathSegment(VECTOR(0, 0), -n*dot(-n,sp), red, 3);
		//mathSegment(VECTOR(0, 0), -n, red, 3);
		//mathSegment(VECTOR(0, 0), sp, red, 3);

		//テキスト情報
		float size = 50;
		float rowH = size + 10;
		float colL = 10;
		int num=0;
		textSize(size);
		if(operateObjSw==0)	
			text((let)"直線の", colL, ++num*rowH);
		else
			text((let)" 点 の", colL, ++num * rowH);
		if (isPress(KEY_SHIFT))
			text((let)"回転 : shift + AD", colL + size * 3, num * rowH);
		else
			text((let)"移動 : ADWS", colL + size * 3, num * rowH);
		text((let)"操作対象切換 : Z", colL, ++num * rowH);
		text((let)"原点までの最短距離:" + c, colL, ++num*rowH);
	}
}
