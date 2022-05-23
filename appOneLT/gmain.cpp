//テスト中
#include"libOne.h"
#include"view_proj.h"
#include"segment.h"
#include"triangle.h"
#include"axis.h"
#include"squareWithHole.h"
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
    //線分のオリジナルポジションosp,oep
    //VECTOR osp(0,  0, 0);//original start point
    //線分の座標変換後のポジションsp,ep
    //VECTOR sp;//start point
    VECTOR segTran(0.5f,0.3f,0.0f);//セグメントの移動用 segment translate
    VECTOR segRot;//セグメントの回転用 segment rotate

    //-----------------------------------------------------------------------
    //三角形頂点のオリジナルポジションop[3]と法線ベクトルonv
    VECTOR op[2][3];
    VECTOR onv[2];
    //反時計回りに頂点を用意する
    float y = -0.1f;
    op[0][0].set(0,y,-1);
    op[0][1].set(-1,0,0);
    op[0][2].set(0,y,1);
    onv[0] = cross(op[0][1] - op[0][0], op[0][2] - op[0][0]);
    onv[0].normalize();

    op[1][0].set(0, y, 1);
    op[1][1].set(1, 0, 0);
    op[1][2].set(0, y, -1);
    onv[1] = cross(op[1][1] - op[1][0], op[1][2] - op[1][0]);
    onv[1].normalize();

    //三角形頂点の座標変換後のポジションp[3]と法線ベクトルnv
    VECTOR p[2][3];
    VECTOR nv[2];
    VECTOR triTran;//三角形の移動用 triangle translate
    VECTOR triRot;//三角形の回転用 triangle rotate
    //-----------------------------------------------------------------------
    //線分と三角形を座標変換するための共用データ
    MATRIX world;
    float speed = 0.005f;
    //-----------------------------------------------------------------------
    //点から三角ポリゴンを含む無限平面までの距離
    float distance = 0;

    //その他------------------------------------------------------------------
    //色
    COLOR red(255, 0, 0, 200);
    COLOR grayLight(150, 150, 150, 230);
    COLOR gray(128, 128, 128, 160);
    COLOR white(255, 255, 255);
    COLOR green(0, 255, 0);
    COLOR yellow(255, 255, 60);
    COLOR cyan(0, 255, 255);
    COLOR blue(255, 0, 255);
    COLOR triColor[2];//三角形の色
    COLOR crossColor[3]  { yellow,green,cyan };

    //表示フラッグ
    bool dispAxisFlag = false;
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
                segTran.set(0.5f,0.3f,0.0f);
                segRot.set(0, 0, 0);
                triTran.set(0, 0, 0);
                triRot.set(0, 0, 0);
            }
        }
        //線分を動かす---------------------------------------------------------
        {
            if (operateObjSw == 0) {
                input(segTran, segRot, speed);
            }
            //world.identity();
            //world.mulTranslate(segTran);
            //world.mulRotateYXZ(segRot);
            //sp = world * osp;
        }
        //三角形の頂点を動かす--------------------------------------------------
        {
            if (operateObjSw == 1) {
                input(triTran, triRot, speed);
            }
            world.identity();
            world.mulTranslate(triTran);
            world.mulRotateYXZ(triRot);
            for (int j = 0; j < 2; j++) {
                for (int i = 0; i < 3; i++) {
                    p[j][i] = world * op[j][i];
                }
            }
            //三角形の法線を回転させる
            world.identity();
            world.mulRotateYXZ(triRot);
            for (int j = 0; j < 2; j++) {
                nv[j] = world * onv[j];
            }
        }
        //当たり判定----------------------------------------------------------
        {
            bool flag = false;
            for (int j = 0; j < 2; j++) {
                distance = dot(nv[j], segTran - p[j][0]);
                triColor[j] = grayLight;//当たっていないときグレイ
                bool containFlag = true;//とりあえず含まれていることにする
                for (int i = 0; i < 3; i++) {
                    VECTOR side = p[j][(i + 1) % 3] - p[j][i];//三角形の一辺のベクトル
                    VECTOR p_segTran = segTran - p[j][i];//三角形の１つの頂点から交差点までのベクトル
                    VECTOR c = cross(side, p_segTran);
                    float d = dot(nv[j], c);
                    if (d < 0) {//外積ベクトルが法線ベクトルと逆方向になっている
                        containFlag = false;//ipが三角形に含まれていない
                    }
                    //外積ベクトル表示
                    //if (dispCrossFlag) {
                    //    float thickness = 1.2f;
                    //    segment(p[i], p[i] + c, crossColor[i], thickness);//外積ベクトル表示
                    //    segment(p[i], p[(i + 1) % 3], crossColor[i], thickness);//辺ベクトル表示
                    //    segment(p[i], segTran, crossColor[i], thickness);//交点までのベクトル
                    //}
                }
                if (containFlag) {
                    flag = true;
                    triColor[j] = red;
                    segTran = segTran + nv[j] * (0.1f - distance);
                }
            }
            if (flag == false) {
                VECTOR v1 = p[0][2] - p[0][0];
                v1.normalize();
                VECTOR v2 = segTran - p[0][0];
                VECTOR v3 = cross(v1, v2);
                float dist = v3.mag();
                if (dist < 0.1f) {
                    v3.normalize();
                    VECTOR v4 = cross(v3, v1);
                    segTran = segTran + v4 * (0.1f - dist);
                }
                text(dist, 900, 100);
            }
        }
        //描画----------------------------------------------------------------
        {
            if (dispAxisFlag) { 
                axis(white, 0.4f);
            }
            //segment(segTran, segTran+(-nv*distance), white, 1.5f);
            point(segTran, white);
            
            if (dispSquareFlag) { 
                squareWithHole(triTran, triRot, gray);
            }
            
            triangle(p[0], triColor[0]);
            triangle(p[1], triColor[1]);

            //text info
            float size = 30;
            textSize(size);
            float colL = 10;//列の始まり
            float rowH = size + 10;//行の高さ
            int num = 0;//行番号

            if (operateObjSw == 0)
                text("線分の", colL, ++num * rowH);
            else 
                text("三角の",colL, ++num * rowH);

            if (isPress(KEY_SHIFT))
                text("回転 : shift+ADWSQE", colL+size * 3, num * rowH);
            else 
                text("移動 : ADWSQE", colL+size*3, num * rowH);
            
            text("操作対象切換 : Z", colL, ++num * rowH);
            text("位置回転リセット : R", colL, ++num * rowH);
            text("軸表示 : X", colL, ++num * rowH);
            text("外積表示 : C", colL, ++num * rowH);
            textSize(20);
            //text((let)"三角形の法線:" + nv.x + " " + nv.y + " " + nv.z, colL, ++num * rowH);
        }
    }
}
