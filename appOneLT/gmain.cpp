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
    createSegment();
    createPoint();

    //-----------------------------------------------------------------------
    //線分のオリジナルポジション始点osp、終点oep
    VECTOR osp(0,  0.3f, 0);
    VECTOR oep(0, -0.3f, 0);
    //座標変換後の線分の始点sp、終点ep
    VECTOR sp;
    VECTOR ep;
    VECTOR segTran(0.6f,0.6f,0);//ベクトルaの移動用
    VECTOR segRot;//ベクトルaの回転用

    //-----------------------------------------------------------------------
    //平面オリジナルポジションop
    float l = 0.8f;
    VECTOR op[4] = {
        VECTOR(-l, 0, l),
        VECTOR(l, 0, l),
        VECTOR(l, 0, -l),
        VECTOR(-l, 0, -l),
    };
    //座標変換後の平面ポジションsqp
    VECTOR p[4];
    VECTOR sqTran;//ベクトルnの移動用
    VECTOR sqRot;//ベクトルnの回転用
    //ベクトルｎのオリジナルポジションonv
    VECTOR onv(0, 1, 0);//original normal vector
    //座標変換後のベクトルnv
    VECTOR nv;//normal vector
    //交点の座標
    VECTOR ip;

    //-----------------------------------------------------------------------
    //線分と三角形を座標変換するための共用データ
    float speed = 0.006f;

    //その他------------------------------------------------------------------
    //色
    COLOR yellow(255, 255, 120);
    COLOR cyan(0, 255, 255);
    COLOR white(255, 255, 255);
    COLOR white2(160, 160, 160);
    COLOR pink(255, 100, 100);
    COLOR green(0, 255, 0);
    COLOR squareColor;
    COLOR gray(128, 128, 128, 160);
    COLOR red(255, 0, 0, 160);
    //表示フラッグ
    bool dispAxisFlag = false;
    bool dispIpFlag = false;
    //表示スィッチ
    int dispSw = 0;
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
            segTran.set(0.6f, 0.6f, 0);
            segRot = sqTran = sqRot = VECTOR(0, 0, 0);
        }
        //線分を動かす---------------------------------------------------------
        if (operateObjSw == 0) {
            input(segTran, segRot, speed);
        }
        gWorld.identity();
        gWorld.mulTranslate(segTran);
        gWorld.mulRotateYXZ(segRot);
        sp = gWorld * osp;
        ep = gWorld * oep;
        //平面を動かす--------------------------------------------------
        if (operateObjSw == 1) {
            input(sqTran, sqRot, speed);
        }
        gWorld.identity();
        gWorld.mulTranslate(sqTran);
        gWorld.mulRotateYXZ(sqRot);
        for (int i = 0; i < 4; i++) {
            p[i] = gWorld * op[i];
        }
        //法線を回転させる。移動してはだめ。
        gWorld.identity();
        gWorld.mulRotateYXZ(sqRot);
        nv = gWorld * onv;
        //衝突判定----------------------------------------------------------------
        //始点と終点が平面をはさんでいるか
        VECTOR v1 = sp - sqTran;
        VECTOR v2 = ep - sqTran;
        float d1 = dot(nv, v1);
        float d2 = dot(nv, v2);
        //d1,d2の符号が違えばはさんでいる
        if (d1 * d2 <= 0) {
            //内分点ipの座標を求める
            float m = Abs(d1);
            float n = Abs(d2);
            ip = (sp * n + ep * m) / (m + n);
            squareColor = red;
            dispIpFlag = true;
        }
        else {
            squareColor = gray;
            dispIpFlag = false;
        }
        //描画----------------------------------------------------------------
        if (dispAxisFlag) axis(white, 1);
        segment(sp, ep, cyan, 10);
        point(sp, pink, 30);
        point(ep, green, 30);
        if(dispIpFlag)point(ip, white,30);
        square(p, squareColor);
        //説明用ベクトル描画
        VECTOR ofst(0, 0, 0);
        if (isTrigger(KEY_C))++dispSw %= 6;
        switch (dispSw) {
        case 5:
            segment(ep, (sqTran + nv * d2), green, 2);
            segment(sqTran + ofst * 2, (sqTran + nv * d2) + ofst * 2, green, 10);
        case 4:
            segment(sp, (sqTran + nv * d1), pink, 2);
            segment(sqTran + ofst, (sqTran + nv * d1) + ofst, pink, 10);
        case 3:
            segment(sqTran, sqTran + nv, yellow, 4);//法線
        case 2:
            segment(sqTran, ep, green, 5);//ｅｐへのベクトル
        case 1:
            segment(sqTran, sp, pink, 5);//ｓｐへのベクトル
        }
        //テキスト情報
        float size = 30;
        textSize(size);
        fill(255);
        float colL = 10;//列の左側
        float rowH = size + 10;//行の高さ
        int num = 0;//行番号
        if (operateObjSw == 0)
            text("線分の", colL, ++num * rowH);
        else
            text("平面の", colL, ++num * rowH);
        if (isPress(KEY_SHIFT))
            text("回転 : shift+ADWSQE", colL+size * 3, num * rowH);
        else
            text("移動 : ADWSQE", colL+size * 3, rowH);
        text("操作対象切換 : Z", colL, ++num * rowH);
        text("位置回転リセット : R", colL, ++num * rowH);
        text("軸表示 : X", colL, ++num * rowH);
        text((let)"d1 * d2 = " + d1 * d2, 800, rowH);
    }
}
