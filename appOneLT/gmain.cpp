#include"libOne.h"
#include"view_proj.h"
#include"segment.h"
#include"triangle.h"
#include"axis.h"
#include"squareWithHole.h"
#include"circle.h"
#include"point.h"

//外積ベクトルが法線となる円を描画。元の円は真上を向いている円
void circle(VECTOR n) {
    //円の頂点をｘ軸回転させる角度-----------------------------
    n.normalize();
    //VECTOR(0,1,0)とｎの内積はcosΘ。つまりn.yとなる
    float radX = Acos(n.y);
    //円の頂点をｙ軸回転させる角度------------------------------
    n.y = 0;//２次元ベクトル化。（ｘ、ｚ平面で考える）
    n.normalize();
    //底辺はdot(VECTOR(0,0,1),n)=n.z;
    //高さはcross(VECTOR(0,0,1),n)=n.x;
    float radY = Atan2(n.x, n.z);
    circle(VECTOR(radX, radY, 0), COLOR(128, 128, 128, 128));
}

void input(VECTOR& tran,VECTOR& rot,float speed)
{
    tran.set(0, 0, 0);
    rot.set(0, 0, 0);
    if (isPress(KEY_SHIFT)) {
        //回転
        if (isPress(KEY_D)) { rot.y = -speed; }
        if (isPress(KEY_A)) { rot.y = speed; }
        if (isPress(KEY_W)) { rot.x = speed; }
        if (isPress(KEY_S)) { rot.x = -speed; }
        if (isPress(KEY_Q)) { rot.z = speed; }
        if (isPress(KEY_E)) { rot.z = -speed; }
    }
    else {
        //移動
        if (isPress(KEY_D)) { tran.x = speed; }
        if (isPress(KEY_A)) { tran.x = -speed; }
        if (isPress(KEY_W)) { tran.z = -speed; }
        if (isPress(KEY_S)) { tran.z = speed; }
        if (isPress(KEY_Q)) { tran.y = speed; }
        if (isPress(KEY_E)) { tran.y = -speed; }
    }
}

void gmain() {
    window(1920, 1080, full);
    hideCursor();

    //-----------------------------------------------------------------------
    //ベクトルa
    VECTOR a(1.0f, 0, 0);
    VECTOR aTran;
    VECTOR aRot;
    //-----------------------------------------------------------------------
    //ベクトルb
    VECTOR b(1.0f, 0, 0);
    VECTOR bTran;
    VECTOR bRot;

    //-----------------------------------------------------------------------
    //ベクトルa,bを座標変換するための共用データ
    MATRIX world;
    float speed = 0.01f;

    //その他------------------------------------------------------------------
    //色
    COLOR red(255, 0, 0, 200);
    COLOR orange(255, 150, 0, 230);
    COLOR white(255, 255, 255);
    COLOR green(0, 255, 0);
    COLOR yellow(255, 255, 60);
    COLOR cyan(0, 255, 255);
    COLOR gray(128, 128, 128, 128);

    //表示フラッグ
    bool dispAxisFlag = true;
    bool dispCircleFlag = false;
    //移動回転させるオブジェクトの選択
    int operateObjSw = 1;
    //プロジェクション行列を作っておく
    createProj();
    createCircle();
    //メインループ-------------------------------------------------------------
    while (notQuit) {
        clear(0);
        //カメラ行列を更新
        updateView();
        //表示切替、操作オブジェクト切り替え--------------------------------------
        {
            if (isTrigger(KEY_X)) { dispAxisFlag = !dispAxisFlag; }
            if (isTrigger(KEY_Z)) { operateObjSw = 1 - operateObjSw; }
            if (isTrigger(KEY_R)) {
                a.set(1, 0, 0);
                b.set(1, 0, 0);
                dispCircleFlag = false;
            }
            if (isTrigger(KEY_F)) {
                a.set(0, 0, -1);
                b.set(0, 0, -1);
                dispCircleFlag = false;
            }
            if (isTrigger(KEY_A) || isTrigger(KEY_S) || isTrigger(KEY_D) ||
                isTrigger(KEY_Q) || isTrigger(KEY_W) || isTrigger(KEY_E)) {
                dispCircleFlag = true;
            }
        }
        //ベクトルaを動かす----------------------------------------------------
        {
            if (operateObjSw == 0) {
                input(aTran, aRot, speed);
            }
            world.identity();
            //world.mulTranslate(aTran);
            world.mulRotateYXZ(aRot);
            a = world * a;
        }
        //ベクトルbを動かす----------------------------------------------------
        {
            if (operateObjSw == 1) {
                input(bTran, bRot, speed);
            }
            world.identity();
            //world.mulTranslate(bTran);
            world.mulRotateYXZ(bRot);
            b = world * b;
        }
        //外積ベクトルを求める--------------------------------------------------
        VECTOR c = cross(a, b);
        //描画----------------------------------------------------------------
        if (dispAxisFlag) {
            axis(white, 0.4f);
        }
        float thickness = 2.2f;
        VECTOR o(0, 0, 0);
        segment(o, a, yellow, thickness);//ベクトルa表示
        point(a, yellow);
        segment(o, b, cyan, thickness);//ベクトルb表示
        point(b, cyan);
        segment(o, c, red, thickness);//外積ベクトルc
        point(c, red);
        if (dispCircleFlag) {
            circle(c);
        }
        //text info
        float size = 30;
        textSize(size);
        float rowH = size + 5;//行の高さ
        int num = 0;//行番号
        if (operateObjSw == 0)
            text("ａの", 0, ++num * rowH);
        else
            text("ｂの", 0, ++num * rowH);
        if (isPress(KEY_SHIFT))
            text("回転 : shift+ADWSQE", size * 2, num * rowH);
        else
            text("移動 : ADWSQE", size * 2, rowH);
        text("操作対象切換 : Z", 0, ++num * rowH);
        text("位置回転リセット : R", 0, ++num * rowH);
        text("軸表示 : X", 0, ++num * rowH);

        text((let)"acos(dot(a,b)):" + Acos(dot(a, b))*TO_DEG, 700, 30);
        text((let)"c mag:" + c.mag(), 700, 100);
        text((let)"sin  :" + Sin(Acos(dot(a, b))), 700, 150);
    }
}
