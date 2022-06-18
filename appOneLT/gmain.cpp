#include"libOne.h"
#include"input_.h"
#include"view_proj.h"
#include"axis.h"
#include"segment.h"
#include"point.h"
#include"capsule.h"
//線分クラス(メンバデータはpublic)
class SEGMENT {
public:
    VECTOR sp;//始点
    VECTOR ep;//終点
    VECTOR v;//始点から終点までのベクトル
    void update(const VECTOR& tran, const VECTOR& rot,
        const VECTOR& osp, const VECTOR& oep) 
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
float calcPointLineDist //最短距離
(
    const VECTOR& p, //点 
    const SEGMENT& s, //線分（直線扱い）
    VECTOR& mp, //点pから直線に下ろした垂線の端点
    float& t //s.vの長さを1とした時の「s.spからmpまでの長さ」の割合
) 
{
    t = 0.0f;
    float dvv = s.v.magSq();//dot(s.v, s.v)と同じ
    if (dvv > 0.0f) {
        t = dot(s.v, p - s.sp) / dvv;
        // 上の式の説明
        // dot(s.v, p-s.sp) は |s.v||p-s.sp|cosΘ
        // dvvは|s.v|の２乗
        // 上の計算で、tは |p-s.sp|cosΘ / |s.v|となる。
        // つまりtは「dotで投影した長さ÷s.vの長さ」という割合になる
    }
    mp = s.sp + s.v * t;
    return (p - mp).mag();
}

//点と線分の最短距離
float calcPointSegmentDist//最短距離
(
    const VECTOR& p, //点
    const SEGMENT& s, //線分
    VECTOR& mp, //点pから線分までの最短距離となる点(始点や終点になることもある)
    float& t //s.vの長さを1とした時の「s.spからmpまでの長さ」の割合
) 
{
    //とりあえず点から直線までの最短距離やmp,tを求めてみる
    float dist = calcPointLineDist(p, s, mp, t);
    //mpが線分の外にある(始点寄り)
    if (t < 0.0f) {
        //点から線分の始点までの距離が最短
        mp = s.sp;
        return (p - mp).mag();
    }
    //mpが線分の外にある(終点寄り)
    if (t > 1.0f) {
        //点から線分の終点までの距離が最短
        mp = s.ep;
        return(p - mp).mag();
    }
    //mpが線分内にある
    //0 <= t <= 1
    return dist;
}

//直線と直線の最短距離
float calcLineLineDist
(
    const SEGMENT& s1,//直線１
    const SEGMENT& s2,//直線２
    VECTOR& mp1, //最短線の端点１
    VECTOR& mp2, //最短線の端点２
    float& t1, //s1.vの長さを1とした時の「s1.spからmp1までの長さ」の割合
    float& t2  //s2.vの長さを1とした時の「s2.spからmp2までの長さ」の割合
)
{
    //2直線が平行
    if (cross(s1.v, s2.v).magSq() < 0.000001f) {
        //線分1の始点から直線2までの最短距離問題に帰着する
        float dist = calcPointLineDist(s1.sp, s2, mp2, t2);
        mp1 = s1.sp;
        t1 = 0.0f;
        return dist;
    }

    //2直線が平行でない
    //互いに垂直になるような最短線の端点mp1,mp2を求める
    //次の順で求めていく t1 -> mp1 -> t2 -> mp2
    //最初のt1を求める式の詳細は下のコメントにある
    /*
    両直線の最短距離を結ぶ線は、両直線に共通の垂線となる。。。その垂線の端点mp1,mp2
    mp1 = s1.sp + s1.v * t1
    mp2 = s2.sp + s2.v * t2

    t2を求める式
    t2 = dot(s2.v,mp1 - s2.sp) / dot(s2.v,s2.v)
    最終的にt1を求めたいので、t2をt1で表現するためmp1を置き換える
    t2 = dot(s2.v, s1.sp + s1.v * t1 - s2.sp) / dot(s2.v, s2.v)
    分配してt1を計算しやすくしておく
    t2 = dot(s2.v, s1.sp - s2.sp + s1.v * t1) / dot(s2.v, s2.v)
    t2 = { dot(s2.v, s1.sp - s2.sp) + dot(s2.v, s1.v) * t1 } / dot(s2.v, s2.v)


    次の式からt1を導く。バカ丁寧に変形していきます。
    0 = dot(s1.v,mp1-mp2)
    =dot(s1.v,(s1.sp+s1.v*t1)-(s2.sp+s2.v*t2))
    =dot(s1.v, s1.sp-s2.sp + s1.v*t1 - s2.v*t2)
    分配してt2を外に出す
    =dot(s1.v, s1.sp-s2.sp) + dot(s1.v,s1.v)*t1 - dot(s1.v,s2.v)*t2 )
    t2を消す
    =dot(s1.v, s1.sp-s2.sp) + dot(s1.v,s1.v)*t1 - dot(s1.v,s2.v)*{ dot(s2.v, s1.sp - s2.sp) + dot(s2.v, s1.v) * t1 } / dot(s2.v, s2.v)
    一番右の分母を消す（dot(s2.v, s2.v)を掛ける）
    =dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp) + dot(s2.v,s2.v)*dot(s1.v,s1.v)*t1 - dot(s1.v,s2.v) * {dot(s2.v, s1.sp - s2.sp) + dot(s2.v, s1.v) * t1}
    中かっこを消す
    =dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp) + dot(s2.v,s2.v)*dot(s1.v,s1.v)*t1 - dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) - dot(s1.v,s2.v) * dot(s2.v,s1.v) * t1
    t1でくくる
    =dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp)  - dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) + { dot(s2.v,s2.v)*dot(s1.v,s1.v) - dot(s1.v,s2.v) * dot(s2.v,s1.v) }* t1
    移項
    -{dot(s2.v,s2.v)*dot(s1.v,s1.v) - dot(s1.v,s2.v) * dot(s2.v,s1.v)}*t1 = dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp)  - dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp)
    t1=にして、整理していく
    t1 = dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp)  - dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) / - {dot(s2.v,s2.v)*dot(s1.v,s1.v) - dot(s1.v,s2.v) * dot(s2.v,s1.v)}
    t1 = -dot(s2.v,s2.v)*dot(s1.v, s1.sp-s2.sp)  + dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) / dot(s2.v,s2.v)*dot(s1.v,s1.v) - dot(s1.v,s2.v) * dot(s2.v,s1.v)
    t1 = dot(s1.v,s2.v) * dot(s2.v, s1.sp - s2.sp) -　dot(s2.v,s2.v) * dot(s1.v, s1.sp-s2.sp)  / dot(s1.v,s1.v) * dot(s2.v,s2.v) - dot(s1.v,s2.v) * dot(s1.v,s2.v)
    */
    float dv1v2 = dot(s1.v, s2.v);
    float dv1v1 = s1.v.magSq();//dot(s1.v,s1.v)と同じ
    float dv2v2 = s2.v.magSq();//dot(s2.v,s2.v)と同じ
    VECTOR vp2p1 = s1.sp - s2.sp;
    t1 = (dv1v2 * dot(s2.v, vp2p1) - dv2v2 * dot(s1.v, vp2p1))
        / (dv1v1 * dv2v2 - dv1v2 * dv1v2);
    mp1 = s1.sp + s1.v * t1;
    t2 = dot(s2.v, mp1 - s2.sp) / dv2v2;
    mp2 = s2.sp + s2.v * t2;
    return (mp2 - mp1).mag();
}

//0～1の間にクランプ(値を強制的にある範囲内にすること)
void clamp0to1(float& v) {
    if (v < 0.0f)  v = 0.0f;
    else if (v > 1.0f)  v = 1.0f;
}

//2線分間の最短距離
float calcSegmentSegmentDist
(
    const SEGMENT& s1,//線分1
    const SEGMENT& s2,//線分2
    VECTOR& mp1, //最短線の端点1(始点や終点になることもある)
    VECTOR& mp2, //最短線の端点2(始点や終点になることもある)
    float& t1, //s1.vの長さを1とした時の「s1.spからmp1までの長さ」の割合
    float& t2  //s2.vの長さを1とした時の「s2.spからmp2までの長さ」の割合
)
{
    float dist = 0;

    {
        // s1.vが縮退している？
        if (s1.v.magSq() < 0.000001f) {
            // s2.vも縮退？
            if (s2.v.magSq() < 0.000001f) {
                // 点と点の距離の問題に帰着
                dist = (s2.sp - s1.sp).mag();
                mp1 = s1.sp;
                mp2 = s2.sp;
                t1 = t2 = 0.0f;
                segment(mp1, mp2, COLOR(255, 255, 255), 10);
                return dist;
            }
            else {
                // s1.spとs2.vの最短問題に帰着
                dist = calcPointSegmentDist(s1.sp, s2, mp2, t2);
                mp1 = s1.sp;
                t1 = 0.0f;
                clamp0to1(t2);
                segment(mp1, mp2, COLOR(255, 255, 255), 10);
                return dist;
            }
        }
        // s2.vが縮退している？
        else if (s2.v.magSq() < 0.000001f) {
            // s2.spとsv1の最短問題に帰着
            float dist = calcPointSegmentDist(s2.sp, s1, mp1, t1);
            mp2 = s2.sp;
            clamp0to1(t1);
            t2 = 0.0f;
            segment(mp1, mp2, COLOR(255, 255, 255), 10);
            return dist;
        }
    }

    //----------------------------------------------------------------
    //とりあえず2直線間の最短距離,mp1,mp2,t1,t2を求めてみる
    dist = calcLineLineDist(s1, s2, mp1, mp2, t1, t2);
    if (0.0f <= t1 && t1 <= 1.0f &&
        0.0f <= t2 && t2 <= 1.0f) {
        //mp1,mp2が両方とも線分内にあった
        return dist;
    }
    //mp1,mp2の両方、またはどちらかが線分内になかったので次へ


    //----------------------------------------------------------------
    //mp1,t1を求め直す ⇒ t2を0～1にクランプしてmp2からs1.vに垂線を降ろしてみる
    clamp0to1(t2);
    mp2 = s2.sp + s2.v * t2;
    dist = calcPointSegmentDist(mp2, s1, mp1, t1);
    if (0.0f <= t1 && t1 <= 1.0f) {
        //mp1が線分内にあった
        return dist;
    }
    //mp1が線分内になかったので次へ

    //----------------------------------------------------------------
    //mp2,t2を求め直す ⇒ t1を0～1にクランプしてmp1からs2.vに垂線を降ろしてみる
    clamp0to1(t1);
    mp1 = s1.sp + s1.v * t1;
    dist = calcPointSegmentDist(mp1, s2, mp2, t2);
    if (0.0f <= t2 && t2 <= 1.0f) {
        //mp2が線分内にあった
        return dist;
    }
    //mp2が線分内になかったので次へ

    //----------------------------------------------------------------
    //ここまで来たら、mp1からmp2までが最短
    return (mp2 - mp1).mag();
}

//補助線バージョン
COLOR Yellow(255, 255, 0);
COLOR Red(255, 0, 0);
COLOR Green(0, 255, 0);
COLOR Cyan(0, 255, 255);
float calcSegmentSegmentDist_
(
    const SEGMENT& s1,//線分1
    const SEGMENT& s2,//線分2
    VECTOR& mp1, //最短線の端点1(始点や終点になることもある)
    VECTOR& mp2, //最短線の端点2(始点や終点になることもある)
    float& t1, //s1.vの長さを1とした時の「s1.spからmp1までの長さ」の割合
    float& t2  //s2.vの長さを1とした時の「s2.spからmp2までの長さ」の割合
)
{
    float dist = 0;

    //------------------------------------------------------------------------
    //check1:とりあえず2直線間の最短距離,mp1,mp2,t1,t2を求めてみる
    dist = calcLineLineDist(s1, s2, mp1, mp2, t1, t2);
    fill(Yellow);
    if (0.0f <= t1 && t1 <= 1.0f &&
        0.0f <= t2 && t2 <= 1.0f) {
        //mp1,mp2が両方とも線分内にあった
        print((let)"check1〇：t1="+t1+" t2="+t2);
        point(mp1, Yellow, 30);
        point(mp2, Yellow, 30);
        segment(mp1, mp2, Yellow, 10);
        return dist;
    }
    print((let)"check1×：t1="+t1+" t2="+t2);
    segment(mp1, mp2, Yellow, 2);
    point(mp1, Yellow, 12);
    point(mp2, Yellow, 12);
    //mp1,mp2の両方、またはどちらかが線分内になかったので次へ

    //------------------------------------------------------------------------
    //check2:mp1,t1を求め直す ⇒ t2を0～1にクランプしてmp2からs1.vに垂線を降ろしてみる
    clamp0to1(t2);
    mp2 = s2.sp + s2.v * t2;
    dist = calcPointSegmentDist(mp2, s1, mp1, t1);
    fill(Red);
    if (0.0f <= t1 && t1 <= 1.0f) {
        //mp1が線分内にあった
        print((let)"check2〇：t1=" + t1 + " t2=" + t2);
        point(mp1, Red, 30);
        segment(mp1, mp2, Red, 10);
        return dist;
    }
    print((let)"check2×：t1=" + t1 + " t2=" + t2);
    segment(s1.sp+s1.v*t1, mp2, Red, 2);
    point(s1.sp + s1.v * t1, Red, 12);
    //mp1が線分内になかったので次へ

    //------------------------------------------------------------------------
    //check3:mp2,t2を求め直す ⇒ t1を0～1にクランプしてmp1からs2.vに垂線を降ろしてみる
    clamp0to1(t1);
    mp1 = s1.sp + s1.v * t1;
    dist = calcPointSegmentDist(mp1, s2, mp2, t2);
    fill(Green);
    if (0.0f <= t2 && t2 <= 1.0f) {
        //mp2が線分内にあった
        print((let)"check3〇：t1=" + t1 + " t2=" + t2);
        segment(mp1, mp2, Green, 10);
        point(mp2, Green, 30);
        return dist;
    }
    print((let)"check3×：t1=" + t1 + " t2=" + t2);
    segment(mp1, s2.sp + s2.v * t2, Green, 2);
    point(s2.sp + s2.v * t2, Green, 12);
    //mp2が線分内になかったので次へ

    //-----------------------------------------------------------------------
    //ここまで来たら、mp1からmp2までが最短
    fill(Cyan);
    clamp0to1(t2);
    print((let)"last  　：t1=" + t1 + " t2=" + t2);
    segment(mp1, mp2, COLOR(Cyan), 10);
    return (mp2 - mp1).mag();
}

void gmain() {
    window(1920, 1080, full);
    hideCursor();
    createSegment();
    createPoint();
    createCapsule();
    //-----------------------------------------------------------------------
    //カプセル1半径
    float radius1 = 0.2f;
    //線分１のオリジナルポジション
    VECTOR osp1(0, 0.2f, 0);//original start point
    VECTOR oep1(0, -osp1.y, 0);//original end point
    //線分１座標変換用データ
    VECTOR seg1Tran(0.0f, 0.0f, 0.0f);//セグメントの移動用 segment translate
    VECTOR seg1Rot;//セグメントの回転用 segment rotate
    //線分１(座用変換後の値をこれにセットする)
    SEGMENT s1;
    //-----------------------------------------------------------------------
    //カプセル2半径
    float radius2 = 0.2f;
    //線分2のオリジナルポジション
    VECTOR osp2(0, 0.2f, 0);//original start point
    VECTOR oep2(0, -osp2.y, 0);//original end point
    //線分１座標変換用データ
    VECTOR seg2Tran(0.5f, 0.0f, 0.0f);//セグメントの移動用 segment translate
    VECTOR seg2Rot;//セグメントの回転用 segment rotate
    //線分１(座用変換後の値をこれにセットする)
    SEGMENT s2;
    //-----------------------------------------------------------
    //移動回転スピード
    float speed = 0.3f;
    //表示フラッグ
    bool dispAxisFlag = false;
    //移動回転させるオブジェクトの選択
    int operateObjSw = 0;
    //その他------------------------------------------------------------------
    //色
    COLOR yellow(255, 255, 0);
    COLOR white(255, 255, 255);
    COLOR colColor(255, 0, 0, 130);
    COLOR noColColor(255, 255, 255, 130);
    COLOR capsuleColor = noColColor;
    //プロジェクション行列を作っておく
    createProj();
    //デルタタイム初期化
    initDeltaTime();
    //メインループ-------------------------------------------------------------
    while (notQuit) {
        //デルタタイム設定
        setDeltaTime();
        //更新中のデータを表示するため、ここでクリア
        clear(60,120,240);
        //カメラ行列を更新
        updateView();
        //表示切替、操作オブジェクト切り替え、トランスフォームリセット----------------
        {
            if (isTrigger(KEY_X)) { dispAxisFlag = !dispAxisFlag; }
            if (isTrigger(KEY_Z)) { operateObjSw = 1 - operateObjSw; }
            if (isTrigger(KEY_R)) {
                seg1Tran.set(0, 0, 0); seg1Rot.set(0, 0, 0);
                seg2Tran.set(0.5f, 0.0f, 0.0f); seg2Rot.set(0, 0, 0);
            }
        }
        //線分1を動かす-----------------------------------------------------------
        {
            if (operateObjSw == 1) {
                input(seg1Tran, seg1Rot, speed*delta);
            }
            s1.update(seg1Tran, seg1Rot, osp1, oep1);
        }
        //線分2を動かす---------------------------------------------------------
        {
            if (operateObjSw == 0) {
                input(seg2Tran, seg2Rot, speed*delta);
            }
            s2.update(seg2Tran, seg2Rot, osp2, oep2);
        }
        //当たり判定----------------------------------------------------------
        {
            VECTOR mp1;
            VECTOR mp2;
            float t1;
            float t2;
            float dist = 0;
            {
                //ステップごとの解説用
                //dist = calcPointLineDist(s2.sp, s1, mp1, t1);
                //point(s2.sp, yellow, 40);
                //segment(s2.sp, mp1, yellow, 10);
                //segment(s1.sp,s1.ep, white, 10);
                //print((let)"t="+t1);
                //segment(s1.sp - s1.v * 5, s1.ep + s1.v * 5, white, 10);

                //dist = calcPointSegmentDist(s2.sp, s1, mp1, t1);
                //print((let)"t="+t1);
                //segment(s1.sp+s1.v*t1, s2.sp, yellow, 5);
                //point(s2.sp, yellow, 40);
                //segment(s2.sp, mp1, yellow, 10);
                //segment(s1.sp,s1.ep, white, 10);

                //dist = calcLineLineDist(s1, s2, mp1, mp2, t1, t2);
                //segment(mp2, mp1, yellow, 10);
                //point(mp1, yellow, 40);
                //point(mp2, yellow, 40);
                //segment(s1.sp - s1.v * 5, s1.sp + s1.v * 5, white, 10);
                //segment(s2.sp - s2.v * 5, s2.sp + s2.v * 5, white, 10);
            }
            dist = calcSegmentSegmentDist_(s1, s2, mp1, mp2, t1, t2);
            print((let)"distance="+dist);
            if (dist < radius1+radius2) capsuleColor = colColor;
            else capsuleColor = noColColor;
        }
        //描画----------------------------------------------------------------
        {
            //軸
            if(dispAxisFlag)axis(white, 1);
            //線分１
            s1.draw();
            //線分２
            s2.draw();
            //カプセル
            capsule(seg2Tran, seg2Rot, capsuleColor, osp2.y, radius2);
            capsule(seg1Tran, seg1Rot, capsuleColor, osp1.y, radius1);

            //テキスト情報
            //float size = 30;
            //textSize(size);
            //float colL = 10;//列の始まり
            //float rowH = size + 10;//行の高さ
            //int num = 0;//行番号
            //fill(255);
            //text((let)"", colL, ++num * rowH);
        }
    }
}
