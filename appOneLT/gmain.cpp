#include"libOne.h"
#include"input_.h"
#include"view_proj.h"
#include"axis.h"
#include"segment.h"
#include"point.h"
struct SEGMENT {
    VECTOR sp;
    VECTOR ep;
    VECTOR v;
    void set(const VECTOR& startPos, const VECTOR& endPos) 
    {
        sp = startPos; 
        ep = endPos;
        v = ep - sp;
    }
    void set(const VECTOR& tran, const VECTOR& rot,const VECTOR& osp, const VECTOR& oep) 
    {
        gWorld.identity();
        gWorld.mulTranslate(tran);
        gWorld.mulRotateYXZ(rot);
        sp = gWorld * osp;
        ep = gWorld * oep;
        v = ep - sp;
    }
    void draw() 
    {
        segment(sp, ep, COLOR(255, 255, 255), 10);
    }
};
//点と直線の最短距離
float calcPointLineDist//最短距離
(
    const VECTOR& p,//点 
    const SEGMENT& s,
    VECTOR& mp, //点から下ろした垂線の端点
    float& t //ベクトル係数
) 
{
    float segMagSq = s.v.magSq();
    t = 0.0f;
    if (segMagSq > 0.0f) {
        t = dot(s.v, p - s.sp) / segMagSq;
    }
    mp = s.sp + s.v * t;
    return (mp - p).mag();
}
// 点と線分の最短距離
float calcPointSegmentDist//最短距離
(
    const VECTOR& p, //点
    const SEGMENT& s,
    VECTOR& mp, //最短距離となる端点
    float& t //ベクトル係数
) 
{
    // 垂線の長さlen、垂線の足の座標mp及びtを算出
    float len = calcPointLineDist(p, s, mp, t);
    if (t < 0.0f) {
        mp = s.sp;
        return (p - mp).mag();
    }
    if (t > 1.0f) {
        mp = s.ep;
        return(p - mp).mag();
    }
    return len;
}
//直線と直線の最短距離
float calcLineLineDist
(
    const SEGMENT& s1,
    const SEGMENT& s2,
    VECTOR& mp1, 
    VECTOR& mp2,
    float& t1, 
    float& t2
) 
{
    // 2直線が平行
    if (cross(s1.v, s2.v).magSq() < 0.000001f) {
        //線分1の始点と直線2の最短距離の問題に帰着
        float len = calcPointLineDist(s1.sp, s2, mp2, t2);
        mp1 = s1.sp;
        t1 = 0.0f;
        return len;
    }

    // 2直線はねじれ関係
    float dsv1sv2 = dot(s1.v, s2.v);
    float sv1magSq = s1.v.magSq();
    float sv2magSq = s2.v.magSq();
    VECTOR sp2sp1 = s1.sp - s2.sp;
    t1 = (dsv1sv2 * dot(s2.v,sp2sp1) - sv2magSq * dot(s1.v,sp2sp1))
        / (sv1magSq * sv2magSq - dsv1sv2 * dsv1sv2);
    mp1 = s1.sp + s1.v * t1;
    t2 = dot(s2.v, mp1 - s2.sp) / sv2magSq;
    mp2 = s2.sp + s2.v * t2;
    return (mp2 - mp1).mag();
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
    const SEGMENT& s1,
    const SEGMENT& s2,
    VECTOR& mp1, 
    VECTOR& mp2, 
    float& t1, 
    float& t2
) {
    float len = 0;

    /*
    // sv1が縮退している？
    if (sv1.magSq() < 0.000001f) {
        // sv2も縮退？
        if (sv2.magSq() < 0.000001f) {
            // 点と点の距離の問題に帰着
            len = (sp2 - sp1).mag();
            mp1 = sp1;
            mp2 = sp2;
            t1 = t2 = 0.0f;
            segment(mp1, mp2, COLOR(255, 255, 255), 10);
            return len;
        }
        else {
            // sp1とsv2の最短問題に帰着
            len = calcPointSegmentDist(sp1, sp2, ep2, mp2, t2);
            mp1 = sp1;
            t1 = 0.0f;
            clamp01(t2);
            segment(mp1, mp2, COLOR(255, 255, 255), 10);
            return len;
        }
    }
    // sv2が縮退している？
    else if (sv2.magSq() < 0.000001f) {
        // sp2とsv1の最短問題に帰着
        float len = calcPointSegmentDist(sp2, sp1, ep1, mp1, t1);
        mp2 = sp2;
        clamp01(t1);
        t2 = 0.0f;
        segment(mp1, mp2, COLOR(255, 255, 255), 10);
        return len;
    }
    */

    /* 線分同士 */

    // 2直線間の最短距離を求めて仮のt1,t2を求める
    len = calcLineLineDist(s1, s2, mp1, mp2, t1, t2);
    if (0.0f <= t1 && t1 <= 1.0f &&
        0.0f <= t2 && t2 <= 1.0f) {
        segment(mp1, mp2, COLOR(255, 255, 0), 10);
        return len;
    }

    // 垂線の足が外にある事が判明（平行でもここに来ることあるよ）----------------
    // t2を0～1にクランプしてsv1に垂線を降ろす
    clamp01(t2);
    mp2 = s2.sp + s2.v * t2;
    len = calcPointSegmentDist(mp2, s1, mp1, t1);
    if (0.0f <= t1 && t1 <= 1.0f) {
        segment(mp1, mp2, COLOR(0, 0, 255), 10);
        return len;
    }
    // t1を0～1にクランプしてsv2に垂線を降ろす
    clamp01(t1);
    mp1 = s1.sp + s1.v * t1;
    len = calcPointSegmentDist(mp1, s2, mp2, t2);
    if (0.0f <= t2 && t2 <= 1.0f) {
        segment(mp1, mp2, COLOR(0, 255, 0), 10);
        return len;
    }

    // 双方の端点が最短と判明-----------------------------------------------
    clamp01(t1);
    mp1 = s1.sp + s1.v * t1;
    segment(mp1, mp2, COLOR(255, 0, 255), 10);
    return (mp2 - mp1).mag();
}

void gmain() {
    window(1920, 1080, full);
    hideCursor();
    createSegment();
    createPoint();
    //-----------------------------------------------------------------------
    //線分１のオリジナルポジション
    VECTOR osp1(0, 0.4f, 0);//original start point
    VECTOR oep1(0, -0.4f, 0);//original end point
    //線分１の座用変換後のポジション
    SEGMENT s1;
    VECTOR seg1Tran(0.0f, 0.0f, 0.0f);
    VECTOR seg1Rot;
    //-----------------------------------------------------------------------
    //線分2のオリジナルポジション
    VECTOR osp2(0, 0.4f, 0);//original start point
    VECTOR oep2(0, -0.4f, 0);//original end point
    //線分2の座標変換後のポジション
    SEGMENT s2;
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
            s1.set(seg1Tran, seg1Rot, osp1, oep1);
        }
        //線分2を動かす---------------------------------------------------------
        {
            if (operateObjSw == 0) {
                input(seg2Tran, seg2Rot, speed*delta);
            }
            s2.set(seg2Tran, seg2Rot, osp2, oep2);
        }
        //当たり判定----------------------------------------------------------
        {
            VECTOR mp1;
            VECTOR mp2;
            float t1;
            float t2;
            float dist = 0;
            //dist = calcPointLineDist(sp1, sp2, ep2, mp2, t2);
            //dist = calcPointSegmentDist(sp1, sp2, ep2, mp2, t2);
            //dist = calcLineLineDist(sp1, ep1, sp2, ep2, mp1, mp2, t1, t2);
            dist = calcSegmentSegmentDist(s1, s2, mp1, mp2, t1, t2);
            print(dist);
            //if (flag) cylinderColor = transRed;
            //else cylinderColor = transWhite;
        }
        //描画----------------------------------------------------------------
        {
            //軸
            if(dispAxisFlag)axis(white, 1);
            //線分１
            s1.draw();
            //線分２
            s2.draw();
            //テキスト情報
            //float size = 30;
            //textSize(size);
            //float colL = 10;//列の始まり
            //float rowH = size + 10;//行の高さ
            //int num = 0;//行番号
            //fill(COLOR(255, 255, 255));
            //text((let)"", colL, ++num * rowH);
        }
    }
}
