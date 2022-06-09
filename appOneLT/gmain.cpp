#include"libOne.h"
#include"input_.h"
#include"view_proj.h"
#include"axis.h"
#include"segment.h"
#include"point.h"
#include"sphere.h"
#include"cylinder.h"

bool intersects(
    const VECTOR& p, float radius,
    const VECTOR& sp, const VECTOR& ep, float segLen
) 
{
    VECTOR v(0, 1, 0);
    float cylinderLen = 1.0f;
    VECTOR sv = (ep - sp).normalize();
    float radiusSq = radius * radius;

    VECTOR pv;

    VECTOR _cross = cross(v, sv);
    float len = _cross.sqMag();
    if (len < 0.000001f) {
        //平行ベクトルのとき
        VECTOR v_ = cross(sp - p, v);
        //*n = p - P[i];
        if (v_.magSq() < radiusSq) {
            if (sv.y > 0) {
                if (sp.y + segLen > p.y && sp.y < p.y + cylinderLen) {
                    return true;
                }
            }
            else {
                if (sp.y > p.y && sp.y - segLen < p.y + cylinderLen) {
                    return true;
                }
            }
        }
    }
    else {
        float dv = dot(v, sv);
        float dA = dot(sp - p, v);
        float dB = dot(sp - p, sv);
        float lenA = (dA - dB * dv) / (1.0f - dv * dv);
        float lenB = (dB - dA * dv) / (dv * dv - 1.0f);
        VECTOR qA = p + v * lenA;
        VECTOR qB = sp + sv * lenB;

        segment(p, p + v * lenA, COLOR(255, 255, 0), 10);
        segment(sp, sp + sv * lenB, COLOR(255, 0, 0), 8);
        print(lenA);
        print(lenB);
        point(qA, COLOR(255, 255, 0), 20);
        point(qB, COLOR(255, 0, 0), 20);
        pv = qA - qB;
        segment(qB, qA, COLOR(255, 0, 255), 10);

        if (pv.magSq() < radiusSq) 
        {
            if (0 <= lenA && lenA <= cylinderLen) 
            {
                if (0 <= lenB && lenB <= segLen) 
                {
                    return true;
                }
            }
        }
    }

    return false;
}

// 点と直線の最短距離
float calcPointLineDist//最短距離
(
    const VECTOR& p,//点 
    const VECTOR& sp,//始点
    const VECTOR& ep,//終点
    VECTOR& h, //点から下ろした垂線の端点
    float& t //ベクトル係数
) 
{
    //線分の長さ
    VECTOR sv = ep - sp;
    float segMagSq = sv.magSq();
    //ベクトル係数
    t = 0.0f;
    if (segMagSq > 0.0f) {
        t = dot(sv, p - sp) / segMagSq;
    }
    //垂線の端点
    h = sp + t * sv;

    //segment(p, h, COLOR(0, 255, 255), 10);
    //なんちゃって直線を描画
    //segment(sp - sv * 3, ep + sv * 3, COLOR(255, 255, 255), 10);
    
    return (h - p).mag();
}
// 点と線分の最短距離
float calcPointSegmentDist//最短距離
(
    const VECTOR& p, //点
    const VECTOR& sp,//線分始点
    const VECTOR& ep,//線分終点
    VECTOR& h, //最短距離となる端点
    float& t //ベクトル係数
) {
    // 垂線の長さ、垂線の足の座標及びtを算出
    float len = calcPointLineDist(p, sp, ep, h, t);

    if (dot(p - sp, ep - sp) < 0) {
        //segment(sp, p, COLOR(255, 0, 0), 10);
        
        h = sp;
        return (p - sp).mag();
    }
    else if (dot(p - ep, sp - ep) < 0) {
        //segment(ep, p, COLOR(255, 0, 0), 10);
        
        h = ep;
        return(p - ep).mag();
    }
    
    //segment(h, p, COLOR(255, 0, 0), 10);
    
    return len;
}
//直線と直線の最短距離
float calcLineLineDist
(
    const VECTOR& sp1,//線分1始点
    const VECTOR& ep1,//線分1終点
    const VECTOR& sp2,//線分2始点
    const VECTOR& ep2,//線分2終点
    VECTOR& p1, 
    VECTOR& p2,
    float& t1, 
    float& t2
) 
{
    VECTOR sv1 = ep1 - sp1;
    VECTOR sv2 = ep2 - sp2;

    // 2直線が平行？
    //if (cross(sv1,sv2).magSq()< 0.000001f) {

    //    //線分1の始点と直線2の最短距離の問題に帰着
    //    float len = calcPointLineDist(sp1, sp2, ep2, p2, t2);
    //    p1 = sp1;
    //    t1 = 0.0f;

    //    return len;
    //}

    // 2直線ねじれの関係
    float dsv1sv2 = dot(sv1, sv2);
    float sv1magSq = sv1.magSq();
    float sv2magSq = sv2.magSq();
    VECTOR sp2sp1 = sp1 - sp2;
    t1 = (dsv1sv2 * dot(sv2,sp2sp1) - sv2magSq * dot(sv1,sp2sp1))
        / (sv1magSq * sv2magSq - dsv1sv2 * dsv1sv2);
    p1 = sp1 + sv1 * t1;
    t2 = dot(sv2, p1 - sp2) / sv2magSq;
    p2 = sp2 + sv2 * t2;

    //segment(p1, p2, COLOR(255, 0, 255), 10);

    return (p2 - p1).mag();
}

// 0～1の間にクランプ
void clamp01(float& v) {
    if (v < 0.0f)
        v = 0.0f;
    else if (v > 1.0f)
        v = 1.0f;
}
// 2線分の最短距離
float calcSegmentSegmentDist
(
    const VECTOR& sp1,//線分1始点
    const VECTOR& ep1,//線分1終点
    const VECTOR& sp2,//線分2始点
    const VECTOR& ep2,//線分2終点
    VECTOR& p1, 
    VECTOR& p2, 
    float& t1, 
    float& t2
) {
    VECTOR sv1 = ep1 - sp1;
    VECTOR sv2 = ep2 - sp2;
    /*
    // S1が縮退している？
    if (sv1.magSq() < 0.000001f) {
        // S2も縮退？
        if (sv2.magSq() < 0.000001f) {
            // 点と点の距離の問題に帰着
            float len = (sp2 - sp1).mag();
            p1 = sp1;
            p2 = sp2;
            t1 = t2 = 0.0f;
            segment(p1, p2, COLOR(255, 255, 255), 12);
            return len;
        }
        else {
            // S1の始点とS2の最短問題に帰着
            float len = calcPointSegmentDist(sp1, sp2, ep2, p2, t2);
            p1 = sp1;
            t1 = 0.0f;
            clamp01(t2);
            segment(p1, p2, COLOR(255, 255, 255), 12);
            return len;
        }
    }

    // S2が縮退している？
    else if (sv2.magSq() < 0.000001f) {
        // S2の始点とS1の最短問題に帰着
        float len = calcPointSegmentDist(sp2, sp1, ep1, p1, t1);
        p2 = sp2;
        clamp01(t1);
        t2 = 0.0f;
        segment(p1, p2, COLOR(255, 255, 255), 12);
        return len;
    }
    */


    /* 線分同士 */

    // 2線分が平行だったら垂線の端点の一つをP1に仮決定
    if (cross(sv1, sv2).magSq() < 0.000001f) {
        t1 = 0.0f;
        p1 = sp1;
        float len = calcPointSegmentDist(sp1, sp2, ep2, p2, t2);
        if (0.0f <= t2 && t2 <= 1.0f) {
            segment(p1, p2, COLOR(255, 0, 255), 12);

            return len;
        }
    }
    else {
        // 線分はねじれの関係
        // 2直線間の最短距離を求めて仮のt1,t2を求める
        float len = calcLineLineDist(sp1, ep1, sp2, ep2, p1, p2, t1, t2);
        if (
            0.0f <= t1 && t1 <= 1.0f &&
            0.0f <= t2 && t2 <= 1.0f
            ) {
            segment(p1, p2, COLOR(255, 255, 0), 12);
            return len;
        }
    }

    // 垂線の足が外にある事が判明
    // S1側のt1を0～1の間にクランプして垂線を降ろす
    clamp01(t1);
    p1 = sp1 + sv1 * t1;
    float len = calcPointSegmentDist(p1, sp2, ep2, p2, t2);
    if (0.0f <= t2 && t2 <= 1.0f) {
        segment(p1, p2, COLOR(255, 0, 0), 12);
        return len;
    }

    // S2側が外だったのでS2側をクランプ、S1に垂線を降ろす
    clamp01(t2);
    p2 = sp2 + sv2 * t2;
    len = calcPointSegmentDist(p2, sp1, ep1, p1, t1);
    if (0.0f <= t1 && t1 <= 1.0f) {
        segment(p1, p2, COLOR(0, 255, 0), 12);
        return len;
    }

    // 双方の端点が最短と判明
    clamp01(t1);
    p1 = sp1 + sv1 * t1;
    segment(p1, p2, COLOR(0, 0,255), 12);
    return (p2 - p1).mag();
}

void gmain() {
    window(1920, 1080, full);
    hideCursor();
    createSegment();
    createPoint();
    createCylinder();
    createSphere();
    //-----------------------------------------------------------------------
    //線分１のオリジナルポジション
    VECTOR osp1(0, 0.4f, 0);//original start point
    VECTOR oep1(0, -0.4f, 0);//original end point
    //線分１の座用変換後のポジション
    VECTOR sp1;//start point
    VECTOR ep1;//end point
    VECTOR seg1Tran(0.0f, 0.0f, 0.0f);
    VECTOR seg1Rot;
    //-----------------------------------------------------------------------
    //線分2のオリジナルポジション
    VECTOR osp2(0, 0.4f, 0);//original start point
    VECTOR oep2(0, -0.4f, 0);//original end point
    //線分2の座標変換後のポジションsp,ep
    VECTOR sp2;//start point
    VECTOR ep2;//end point
    VECTOR seg2Tran(0.5f, 0.0f, 0.0f);//セグメントの移動用 segment translate
    VECTOR seg2Rot;//セグメントの回転用 segment rotate
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
    COLOR cylinderColor = transWhite;
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
                seg2Tran.set(0.5f, 0.0f, 0.0f);
                seg2Rot.set(0, 0, 0);
                seg1Tran.set(0, 0, 0);
                seg1Rot.set(0, 0, 0);
            }
        }
        //線分1を動かす-----------------------------------------------------------
        {
            if (operateObjSw == 1) {
                input(seg1Tran, seg1Rot, speed*delta);
            }
            world.identity();
            world.mulTranslate(seg1Tran);
            world.mulRotateYXZ(seg1Rot);
            sp1 = world * osp1;
            ep1 = world * oep1;
        }
        //線分2を動かす---------------------------------------------------------
        {
            if (operateObjSw == 0) {
                input(seg2Tran, seg2Rot, speed*delta);
            }
            world.identity();
            world.mulTranslate(seg2Tran);
            world.mulRotateYXZ(seg2Rot);
            sp2 = world * osp2;
            ep2 = world * oep2;
        }
        //当たり判定----------------------------------------------------------
        {
            VECTOR h1;
            VECTOR h2;
            float t1;
            float t2;
            float dist = 0;
            //dist = calcPointLineDist(sp1, sp2, ep2, h2, t2);
            //dist = calcPointSegmentDist(sp1, sp2, ep2, h2, t2);
            //dist = calcLineLineDist(sp1, ep1, sp2, ep2, h1, h2, t1, t2);
            dist = calcSegmentSegmentDist(sp1, ep1, sp2, ep2, h1, h2, t1, t2);
            print(dist);
            //if (flag) cylinderColor = transRed;
            //else cylinderColor = transWhite;
        }
        //描画----------------------------------------------------------------
        {
            //軸
            if(dispAxisFlag)axis(white, 1);
            //線分
            segment(sp2, ep2, white, 10);
            //point(sp2, white, 10);
            //point(ep2, white, 10);
            //
            segment(sp1, ep1, white, 10);
            
            //sphere(p, cylinderColor, radius);
            //cylinder(p, cylinderColor, radius);
            
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
