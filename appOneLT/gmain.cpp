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
    //ベクトルaのオリジナルポジションoa
    VECTOR oa(1, 0, 0);
    //座標変換後のベクトルa
    VECTOR a;
    VECTOR aTran;//ベクトルaの移動用
    VECTOR aRot;//ベクトルaの回転用

    //-----------------------------------------------------------------------
    //ベクトルｎのオリジナルポジションon
    VECTOR on(0, 1, 0);
    //座標変換後のベクトルn
    VECTOR n;//normal
    VECTOR nTran;//ベクトルnの移動用
    VECTOR nRot;//ベクトルnの回転用
    //説明用平面オリジナルポジションop
    float l = 1.2f;
    VECTOR op[4] = {
        VECTOR(-l, 0, -l),
        VECTOR(-l, 0, l),
        VECTOR(l, 0, -l),
        VECTOR(l, 0, l),
    };
    //座標変換後の平面ポジションsqp
    VECTOR p[4];
 
    //-----------------------------------------------------------------------
    //線分と三角形を座標変換するための共用データ
    MATRIX world;
    float speed = 0.006f;

    //その他------------------------------------------------------------------
    //色
    COLOR yellow(255, 255, 120);
    COLOR cyan(0, 255, 255);
    COLOR red(255, 0, 0);
    COLOR white(255, 255, 255);
    COLOR gray(128, 128, 128, 190);
    //表示フラッグ
    bool dispAxisFlag = false;
    //移動回転させるオブジェクトの選択
    int operateObjSw = 0;
    
    //プロジェクション行列を作っておく
    createProj();

    //メインループ-------------------------------------------------------------
    while (notQuit) {
        clear(0);
        //カメラ行列を更新
        updateView();
        //表示切替、操作オブジェクト切り替え--------------------------------------
        if (isTrigger(KEY_X)) { dispAxisFlag = !dispAxisFlag; }
        if (isTrigger(KEY_Z)) { operateObjSw = 1 - operateObjSw; }
        if (isTrigger(KEY_R)) {
            aTran = aRot = nTran = nRot = VECTOR(0, 0, 0);
        }
        //ベクトルaを動かす---------------------------------------------------------
        if (operateObjSw == 0) {
            input(aTran, aRot, speed);
        }
        world.identity();
        //world.mulTranslate(aTran);
        world.mulRotateYXZ(aRot);
        a = world * oa;
        //ベクトルnを動かす--------------------------------------------------
        if (operateObjSw == 1) {
            input(nTran, nRot, speed);
        }
        world.identity();
        //world.mulTranslate(nTran);
        world.mulRotateYXZ(nRot);
        n = world * on;
        //説明用平面を動かす
        for (int i = 0; i < 4; i++) {
            p[i] = world * op[i];
        }
        //内積ｄ----------------------------------------------------------------
        float d = dot(n, a);
        //描画----------------------------------------------------------------
        if (dispAxisFlag) {
            axis(white, 0.4f);
        }
        VECTOR o(0, 0, 0);
        segment(o, n, yellow, 1.5f);
        segment(o, a, cyan, 1.5f);
        point(a, cyan);
        segment(o, n * d, red, 3.0f);
        segment(a, n * d, white, 1.0f);
        square(p, gray);

        //text info
        float size = 30;
        textSize(size);
        fill(255);
        float rowH = size + 5;//行の高さ
        int num = 0;//行番号

        if (operateObjSw == 0)
            text("ａの", 0, ++num * rowH);
        else
            text("ｎの", 0, ++num * rowH);

        if (isPress(KEY_SHIFT))
            text("回転 : shift+ADWSQE", size * 2, num * rowH);
        else
            text("移動 : ADWSQE", size * 2, rowH);

        text("操作対象切換 : Z", 0, ++num * rowH);
        text("位置回転リセット : R", 0, ++num * rowH);
        text("軸表示 : X", 0, ++num * rowH);
        ++num;
        textSize(60);
        text((let)"内積ｄ:" + d, size * 22, 65);
        text((let)"acos(d):" + Acos(d)*TO_DEG, size * 44, 65);
    }
}
