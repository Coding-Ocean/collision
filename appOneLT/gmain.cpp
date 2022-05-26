#include"libOne.h"
#include"view_proj.h"
#include"segment.h"
#include"triangle.h"
#include"axis.h"
#include"squareWithHole.h"
#include"point.h"
#include"segment.h"

void input(VECTOR& tran, VECTOR& rot, float speed)
{
    if (isPress(KEY_SHIFT)) {
        //回転
        if (isPress(KEY_D)) { rot.y -= speed; }
        if (isPress(KEY_A)) { rot.y += speed; }
        if (isPress(KEY_W)) { rot.x += speed; }
        if (isPress(KEY_S)) { rot.x -= speed; }
        if (isPress(KEY_Q)) { rot.z += speed; }
        if (isPress(KEY_E)) { rot.z -= speed; }
    }
    else {
        //移動
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
    createSegment();
    createPoint();

    //-----------------------------------------------------------------------
    //点のオリジナルポジションop
    VECTOR op(0, 0, 0);
    //点の座標変換後のポジションp
    VECTOR p;
    VECTOR pTran;//セグメントの移動用 point translate
    VECTOR pRot;//セグメントの回転用 point rotate
    //-----------------------------------------------------------------------
    //三角形頂点のオリジナルポジションotp[3]と法線ベクトルon
    VECTOR otp[3];
    //反時計回りに頂点を用意する
    angleMode(DEGREES);
    otp[0].set(Sin(0) * 0.5f, 0.0f, -Cos(0) * 0.5f);
    otp[1].set(Sin(-120) * 0.5f, 0.0f, -Cos(-120) * 0.5f);
    otp[2].set(Sin(-240) * 0.5f, 0.0f, -Cos(-240) * 0.5f);
    angleMode(RADIANS);
    VECTOR on = cross(otp[1] - otp[0], otp[2] - otp[0]);
    on.normalize();
    //三角形頂点の座標変換後のポジションtp[3]と法線ベクトルn
    VECTOR tp[3];
    VECTOR n;
    VECTOR triTran;//三角形の移動用 triangle translate
    VECTOR triRot(0,0,0.4f);//三角形の回転用 triangle rotate
    //-----------------------------------------------------------------------
    //原点から平面までの最短距離
    float d = 0;
    //-----------------------------------------------------------------------
    //点と三角形を座標変換するための共用データ
    float speed = 0.003f;

    //その他------------------------------------------------------------------
    //色
    COLOR red(255, 0, 0);
    COLOR pink(255, 160, 160);
    COLOR grayLight(140, 140, 140);
    COLOR gray(128, 128, 128);
    COLOR white(255, 255, 255);
    COLOR green(0, 255, 0);
    COLOR yellow(255, 255, 60);
    COLOR cyan(0, 255, 255);
    COLOR blue(255, 0, 255);
    //表示フラッグ
    bool dispAxisFlag = false;
    bool dispNormalFlag = false;
    bool dispSquareFlag = true;
    //移動回転させるオブジェクトの選択
    int operateObjSw = 0;
    //プロジェクション行列を作っておく
    createProj();
    //メインループ-------------------------------------------------------------
    while (notQuit) {
        clear();
        //カメラ行列を更新
        updateView();
        //表示切替、操作オブジェクト切り替え--------------------------------------
        {
            if (isTrigger(KEY_X)) { dispAxisFlag = !dispAxisFlag; }
            if (isTrigger(KEY_C)) { dispNormalFlag = !dispNormalFlag; }
            if (isTrigger(KEY_V)) { dispSquareFlag = !dispSquareFlag; }
            if (isTrigger(KEY_Z)) { operateObjSw = 1 - operateObjSw; }
            if (isTrigger(KEY_R)) {
                pTran.set(0, 0, 0);
                pRot.set(0, 0, 0);
                triTran.set(0, 0, 0);
                triRot.set(0, 0, 0.2f);
            }
        }
        //点を動かす---------------------------------------------------------
        {
            if (operateObjSw == 0) {
                input(pTran, pRot, speed);
            }
            gWorld.identity();
            gWorld.mulTranslate(pTran);
            gWorld.mulRotateYXZ(pRot);
            p = gWorld * op;
        }
        //三角形の頂点を動かす--------------------------------------------------
        {
            if (operateObjSw == 1) {
                input(triTran, triRot, speed);
            }
            //＊＊＊今回は移動してからの回転＊＊＊
            gWorld.identity();
            gWorld.mulRotateYXZ(triRot);
            gWorld.mulTranslate(triTran);
            for (int i = 0; i < 3; i++) {
                tp[i] = gWorld * otp[i];
            }
            //三角形の法線を回転させる
            gWorld.identity();
            gWorld.mulRotateYXZ(triRot);
            n = gWorld * on;
        }
        //平面の式を求め「p.xとp.z」から「p.y」を求める----------------------------
        {
            // 平面の式 ax+by+cz+d=0;
            // ベクトル(a,b,c)は面の法線。dは原点から平面までの最短距離。
            // 未定のdを求める
            // d=-ax-by-cz
            d = -n.x * tp[0].x - n.y * tp[0].y - n.z * tp[0].z;
            // y=(-ax-cz-d)/b
            p.y = (-n.x * p.x - n.z * p.z - d) / n.y;
        }
        //描画----------------------------------------------------------------
        {
            if (dispAxisFlag) {
                //軸
                axis(white, 2);
            }
            if(dispNormalFlag){
                //法線
                segment(VECTOR(0, 0, 0), n, white,6);
                //原点から平面までの最短距離
                segment(VECTOR(0, 0, 0), n * -d, yellow, 7);
            }
            //平面
            triangle(tp, grayLight);
            if (dispSquareFlag) {
                squareWithHole(triTran, triRot, gray);
            }
            //ポイント
            segment(p, p+VECTOR(0,0.1f,0),cyan,9);
            point(p, pink, 20);
            point(VECTOR(p.x,0,p.z), green, 20);
            //テキスト情報
            float size = 30;
            textSize(size);
            float colL = 10;//列の始まり
            float rowH = size + 10;//行の高さ
            int num = 0;//行番号
            if (operateObjSw == 0)
                text(" 点 の", colL, ++num * rowH);
            else
                text("三角の", colL, ++num * rowH);
            if (isPress(KEY_SHIFT))
                text("回転 : shift+ADWSQE", colL + size * 3, num * rowH);
            else
                text("移動 : ADWSQE", colL + size * 3, num * rowH);
            text("操作対象切換 : Z", colL, ++num * rowH);
            text("位置回転リセット : R", colL, ++num * rowH);
            text("軸表示 : X", colL, ++num * rowH);
            text((let)"原点から平面までの最短距離:"+d, colL, ++num * rowH);
        }
    }
}
