#include"libOne.h"
#include"view_proj.h"
#include"segment.h"
#include"triangle.h"
#include"axis.h"
#include"square.h"

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
    VECTOR oa(0.7f, 0, 0);
    VECTOR a;
    VECTOR aTran;//ベクトルaの移動用
    VECTOR aRot;//ベクトルaの回転用

    //-----------------------------------------------------------------------
    VECTOR on(0, 1, 0);
    VECTOR n;
    VECTOR nTran;
    VECTOR nRot;
 
    //-----------------------------------------------------------------------
    //線分と三角形を座標変換するための共用データ
    MATRIX world;
    float speed = 0.006f;

    //その他------------------------------------------------------------------
    //色
    COLOR red(255, 0, 0);
    COLOR orange(255, 150, 0, 230);
    COLOR white(255, 255, 255);
    COLOR gray(128, 128, 128, 128);
    COLOR green(0, 255, 0);
    COLOR yellow(255, 255, 60);
    COLOR cyan(0, 255, 255);
    COLOR triColor;//三角形の色
    COLOR crossColor[3]  { yellow,green,cyan };

    //表示フラッグ
    bool dispAxisFlag = true;
    bool dispSquareFlag = false;
    bool dispCrossFlag = false;
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
        {
            if (isTrigger(KEY_C)) { dispCrossFlag = !dispCrossFlag; }
            if (isTrigger(KEY_X)) { dispAxisFlag = !dispAxisFlag; }
            if (isTrigger(KEY_Z)) { operateObjSw = 1 - operateObjSw; }
            if (isTrigger(KEY_R)) {
                aTran.set(0, 0, 0);
                aRot.set(0, 0, 0);
                nTran.set(0, 0, 0);
                nRot.set(0, 0, 0);
            }
        }
        //ベクトルaを動かす---------------------------------------------------------
        {
            if (operateObjSw == 0) {
                input(aTran, aRot, speed);
            }
            world.identity();
            world.mulRotateYXZ(aRot);
            world.mulTranslate(aTran);
            a = world * oa;
        }
        //ベクトルnを動かす--------------------------------------------------
        {
            if (operateObjSw == 1) {
                input(nTran, nRot, speed);
            }
            world.identity();
            //world.mulTranslate(nTran);
            world.mulRotateYXZ(nRot);
            n = world * on;
        }
        //内積ｄ----------------------------------------------------------------
        float d = dot(n, a);
        //描画----------------------------------------------------------------
        {
            if (dispAxisFlag) {
                axis(white, 0.4f);
            }

            VECTOR o(0, 0, 0);
            segment(o, n, white, 1.5f);
            segment(o, a, cyan, 1.5f);
            segment(o, n * d, red, 3.0f);
            segment(a, n * d, white, 0.2f);

            plane(nTran, nRot, COLOR(180, 120, 120, 180));

            if (dispSquareFlag) {
            }

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
            textSize(70);
            text((let)"内積ｄ:" + d, 0, ++num * rowH);
        }
    }
}
