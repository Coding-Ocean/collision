#include"libOne.h"
#include"view_proj.h"
#include"segment.h"
#include"triangle.h"
#include"axis.h"
#include"square.h"
#include"point.h"

void input(VECTOR& tran,VECTOR& rot,float speed)
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

    //-----------------------------------------------------------------------
    //線分のオリジナルポジション始点osp、終点oep
    VECTOR osp(0,  0.3f, 0);
    VECTOR oep(0, -0.3f, 0);
    //座標変換後の線分の始点sp、終点ep
    VECTOR sp;
    VECTOR ep;
    VECTOR segTran(0,0.4f,0);//ベクトルaの移動用
    VECTOR segRot;//ベクトルaの回転用

    //-----------------------------------------------------------------------
    //平面オリジナルポジションop
    float l = 0.8f;
    VECTOR op[4] = {
        VECTOR(-l, 0, -l),
        VECTOR(-l, 0, l),
        VECTOR(l, 0, -l),
        VECTOR(l, 0, l),
    };
    //座標変換後の平面ポジションsqp
    VECTOR p[4];
    VECTOR sqTran;//ベクトルnの移動用
    VECTOR sqRot;//ベクトルnの回転用
    //ベクトルｎのオリジナルポジションon
    VECTOR on(0, 1, 0);
    //座標変換後のベクトルn
    VECTOR n;//normal
 
    //-----------------------------------------------------------------------
    //線分と三角形を座標変換するための共用データ
    MATRIX world;
    float speed = 0.006f;

    //その他------------------------------------------------------------------
    //色
    COLOR yellow(255, 255, 120);
    COLOR cyan(0, 255, 255);
    COLOR white(255, 255, 255);
    COLOR squareColor;
    COLOR gray(128, 128, 128, 190);
    COLOR red(255, 0, 0, 190);
    //表示フラッグ
    bool dispAxisFlag = false;
    //移動回転させるオブジェクトの選択
    int operateObjSw = 0;
    
    //プロジェクション行列を作っておく
    createProj();

    //メインループ-------------------------------------------------------------
    while (notQuit) {
        clear(60);
        //カメラ行列を更新
        updateView();
        //表示切替、操作オブジェクト切り替え--------------------------------------
        if (isTrigger(KEY_X)) { dispAxisFlag = !dispAxisFlag; }
        if (isTrigger(KEY_Z)) { operateObjSw = 1 - operateObjSw; }
        if (isTrigger(KEY_R)) {
            segTran.set(0, 0.4f, 0);
            segRot = sqTran = sqRot = VECTOR(0, 0, 0);
        }
        //線分を動かす---------------------------------------------------------
        if (operateObjSw == 0) {
            input(segTran, segRot, speed);
        }
        world.identity();
        world.mulTranslate(segTran);
        world.mulRotateYXZ(segRot);
        sp = world * osp;
        ep = world * oep;
        //平面を動かす--------------------------------------------------
        if (operateObjSw == 1) {
            input(sqTran, sqRot, speed);
        }
        world.identity();
        world.mulTranslate(sqTran);
        world.mulRotateYXZ(sqRot);
        for (int i = 0; i < 4; i++) {
            p[i] = world * op[i];
        }
        //法線を動かす
        world.identity();
        world.mulRotateYXZ(sqRot);
        n = world * on;
        //衝突判定----------------------------------------------------------------
        VECTOR v1 = sp - p[0];
        VECTOR v2 = ep - p[0];
        float d1 = dot(n, v1);
        float d2 = dot(n, v2);
        if (d1 * d2 <= 0) {
            d1 = Abs(d1);
            d2 = Abs(d2);
            float m = d1 / (d1 + d2);
            VECTOR ip = sp * (1 - m) + ep * m;
            VECTOR ofst(0, 0.02f, 0);
            point(ip, cyan);
            squareColor = red;
        }
        else {
            squareColor = gray;
        }
        //描画----------------------------------------------------------------
        if (dispAxisFlag) {
            axis(white, 0.4f);
        }
        segment(sp, ep, yellow, 1.5f);
        square(p, squareColor);

        //text info
        float size = 30;
        textSize(size);
        fill(255);
        float rowH = size + 5;//行の高さ
        int num = 0;//行番号

        if (operateObjSw == 0)
            text("線分の", 0, ++num * rowH);
        else
            text("平面の", 0, ++num * rowH);

        if (isPress(KEY_SHIFT))
            text("回転 : shift+ADWSQE", size * 3, num * rowH);
        else
            text("移動 : ADWSQE", size * 3, rowH);

        text("操作対象切換 : Z", 0, ++num * rowH);
        text("位置回転リセット : R", 0, ++num * rowH);
        text("軸表示 : X", 0, ++num * rowH);
        ++num;
        textSize(60);
        //text((let)"内積ｄ:" + d, size * 22, 65);
    }
}
