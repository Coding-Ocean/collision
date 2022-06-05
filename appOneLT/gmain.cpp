#include"libOne.h"
#include"input_.h"
#include"view_proj.h"
#include"axis.h"
#include"segment.h"
#include"point.h"
#include"sphere.h"
bool intersects(
    const VECTOR& p, float radius, //球
    const VECTOR& sp, const VECTOR& ep, float segLen //線分
) 
{
    float radiusSq = radius * radius;
    VECTOR pv;

    //線分の終点から球体の中心点までのベクトル
    pv = p - ep;
    if (pv.magSq() < radiusSq) return true;

    //線分の始点から球体の中心点までのベクトル
    pv = p - sp;
    if (pv.magSq() < radiusSq) return true;

    //線分(始点から終点へ)の正規化ベクトル
    VECTOR sv = (ep - sp).normalize();
    //svとpvの内積。（pvは始点から中心点までのベクトル）
    float d = dot(sv, pv);
    //内積の値が０より大きく、線分ベクトルの大きさより小さいなら
    if (0 < d && d < segLen) {
        //svを長さdのベクトルにする
        sv *= d;
        //ベクトルの引き算でpvを「線分から球体の中心点までの最短ベクトル」に書き換える
        pv -= sv;
        if (pv.magSq() < radiusSq) return true;
    }

    return false;
}

//解説用ベクトル表示バージョン
bool intersects_(
    const VECTOR& p, float radius, //球
    const VECTOR& sp, const VECTOR& ep, float segLen //線分
)
{
    textSize(50);
    static int sw = 6;
    if (isTrigger(KEY_C)) { --sw; if (sw < 0)sw = 6; }

    float radiusSq = radius * radius;
    VECTOR pv;

    //線分の終点から球体の中心点までのベクトル
    pv = p - ep;
    if (pv.magSq() < radiusSq) {
        if (sw == 5) {
            segment(ep, p, COLOR(255, 255, 0), 12);
            fill(COLOR(255, 255, 0));
            text("pv = p - ep;", 30, 80);
        }
        return true;
    }

    //線分の始点から球体の中心点までのベクトル
    pv = p - sp;
    if (pv.magSq() < radiusSq) {
        if (sw == 5) {
            segment(sp, p, COLOR(255, 255, 0), 12);
            fill(COLOR(255, 255, 0));
            text("pv = p - sp;", 30, 80);
        }
        return true;
    }

    //線分(始点から終点へ)の正規化ベクトル
    VECTOR sv = (ep - sp).normalize();
    //svとpv（始点から中心点までのベクトル)の内積
    float d = dot(sv, pv);

    //解説用ベクトル表示
    {
        text(sw == 5 ? "_" : "", 1900, 50);
        switch (sw) {
        case 0:
        case 1:
        case 2: //内積
            fill(COLOR(255, 255, 255));
            text((let)"d = dot(sv,pv); ⇒ " + d, 30, 200);
        case 3: //線分正規化ベクトル
            segment(sp, sp + sv, COLOR(0, 255, 255), 9);
            fill(COLOR(0, 255, 255));
            text("sv = (ep-sp).normalize();", 30, 140);
        case 4: //始点⇒中心点
            segment(sp, p, COLOR(0, 255, 0), 12);
            fill(COLOR(0, 255, 0));
            text("pv = p - sp;", 30, 80);
        }
    }
    //内積の値が０より大きく、線分の大きさより小さいなら
    if (0 < d && d < segLen) {
        {
            //解説用ベクトル表示
            switch (sw) {
            case 0://最短ベクトル
                segment(sp + sv * d, p, COLOR(255, 255, 0), 12);
                fill(255, 255, 0);
                text("pv -= sv;", 30, 320);
            case 1://正規化ベクトル＊内積
                segment(sp, sp + sv * d, COLOR(255, 0, 255), 12);
                fill(COLOR(255, 0, 255));
                text("sv *= d;", 30, 260);
            }
        }
        //svを長さdのベクトルにする
        sv *= d;
        //ベクトルの引き算でpvを「線分から球体の中心点までの最短ベクトル」に書き換える
        pv -= sv;
        if (pv.magSq() < radiusSq) return true;
    }

    return false;
}

void gmain() {
    window(1920, 1080, full);
    hideCursor();
    createSegment();
    createPoint();
    createSphere();
    //-----------------------------------------------------------------------
    //線分のオリジナルポジションosp,oep
    VECTOR osp(0, 0.4f, 0);//original start point
    VECTOR oep(0, -0.4f, 0);//original end point
    VECTOR w = oep - osp;
    float segLen = w.mag();
    //線分の座標変換後のポジションsp,ep
    VECTOR sp;//start point
    VECTOR ep;//end point
    VECTOR segTran(0.5f, 0.0f, 0.0f);//セグメントの移動用 segment translate
    VECTOR segRot;//セグメントの回転用 segment rotate
    //-----------------------------------------------------------------------
    //球体のオリジナルポジションop
    VECTOR op(0, 0, 0);
    //球体の座用変換後のポジションp
    VECTOR p;
    VECTOR sphTran(0.0f, 0.0f, 0.0f);//球体の移動用 sphere translate
    VECTOR sphRot;
    float radius = 0.3f;
    //移動回転スピード
    MATRIX world;
    float speed = 0.3f;
    //表示フラッグ
    bool dispAxisFlag = false;
    //移動回転させるオブジェクトの選択
    int operateObjSw = 0;
    //その他------------------------------------------------------------------
    //色
    COLOR white(220, 220, 220);
    COLOR transRed(255, 0, 0, 150);
    COLOR transWhite(255, 255, 255,180);
    COLOR sphColor = transWhite;
    //プロジェクション行列を作っておく
    createProj();
    //デルタタイム初期化
    initDeltaTime();
    //メインループ-------------------------------------------------------------
    while (notQuit) {
        //デルタタイム設定
        setDeltaTime();
        //更新中のデータを表示するため、ここでクリア
        clear(30,80,180);
        //カメラ行列を更新
        updateView();
        //表示切替、操作オブジェクト切り替え--------------------------------------
        {
            if (isTrigger(KEY_X)) { dispAxisFlag = !dispAxisFlag; }
            if (isTrigger(KEY_Z)) { operateObjSw = 1 - operateObjSw; }
            if (isTrigger(KEY_R)) {
                segTran.set(0.5f, 0.0f, 0.0f);
                segRot.set(0, 0, 0);
                sphTran.set(0, 0, 0);
                sphRot.set(0, 0, 0);
            }
        }
        //線分を動かす---------------------------------------------------------
        {
            if (operateObjSw == 0) {
                input(segTran, segRot, speed*delta);
            }
            world.identity();
            world.mulTranslate(segTran);
            world.mulRotateYXZ(segRot);
            sp = world * osp;
            ep = world * oep;
        }
        //球を動かす-----------------------------------------------------------
        {
            if (operateObjSw == 1) {
                input(sphTran, sphRot, speed*delta);
            }
            world.identity();
            world.mulTranslate(sphTran);
            //world.mulRotateYXZ(sphRot);
            p = world * op;
        }
        //当たり判定----------------------------------------------------------
        {
            bool flag = intersects(p, radius, sp, ep, segLen);
            if (flag) sphColor = transRed;
            else sphColor = transWhite;
        }
        //描画----------------------------------------------------------------
        {
            //軸
            if(dispAxisFlag)axis(white, 1);
            //線分
            segment(sp, ep, white, 10);
            point(sp, white, 10);
            point(ep, white, 10);
            //球体
            sphere(p, sphColor, radius);
            //テキスト情報
            float size = 30;
            textSize(size);
            float colL = 10;//列の始まり
            float rowH = size + 10;//行の高さ
            int num = 0;//行番号
            fill(COLOR(255, 255, 255));
            text((let)"", colL, ++num * rowH);
        }
    }
}
