#include"libOne.h"
#include"view_proj.h"
#include"segment.h"
#include"axis.h"
#include"squareWithHole.h"
#include"point.h"
#include"segment.h"
#include"TRIANGLE.h"

void gmain() {
    window(1920, 1080, full);
    hideCursor();
    createSegment();
    createPoint();
    //-----------------------------------------------------------------------
    //フィールドを構成する三角ポリゴン（座標は固定）
    const int num = 8;
    TRIANGLE triangles[num];
    VECTOR center;
    //左側の山
    center.set(-1.0f, 0.8f, 0.3f);
    triangles[0].create(center, VECTOR(-2, 0, -1), VECTOR(-2, 0, 1));
    triangles[1].create(center, VECTOR(-2, 0, 1), VECTOR(0, 0, 1));
    triangles[2].create(center, VECTOR(0, 0, 1), VECTOR(0, 0, -1));
    triangles[3].create(center, VECTOR(0, 0, -1), VECTOR(-2, 0, -1));
    //右側の谷
    center.set(1.3f, -0.5f, -0.5f);
    triangles[4].create(center, VECTOR(0, 0, -1), VECTOR(0, 0, 1));
    triangles[5].create(center, VECTOR(0, 0, 1), VECTOR(2, 0, 1));
    triangles[6].create(center, VECTOR(2, 0, 1), VECTOR(2, 0, -1));
    triangles[7].create(center, VECTOR(2, 0, -1), VECTOR(0, 0, -1));
    //-----------------------------------------------------------------------
    //キャラクタ(点)
    VECTOR p;
    float angle = 0;
    int dispCharaSw = 1;
    //その他------------------------------------------------------------------
    //色
    COLOR cyan(0, 255, 255);
    COLOR white(255, 255, 255);
    //プロジェクション行列を作っておく
    createProj();
    //デルタタイム初期化
    initDeltaTime();
    //メインループ-------------------------------------------------------------
    while (notQuit) {
        //デルタタイム設定
        setDeltaTime();
        //更新中のデータを表示するため、ここでクリア
        clear();
        //カメラ行列を更新
        updateView();
        //キャラクタ(点)を楕円運動させる----------------------------------------------------
        {
            //p.xとp.zだけ設定
            angle += 0.6f * delta;
            p.x = Cos(angle) * 1.8f;
            p.z = Sin(angle) * 0.8f;
            //p.yはとりあえずゼロ！！！！
            p.y = 0.0f;
        }
        //三角形が点(p.x p.z)を含んでいたらp.yを取得する--------------------------
        {
            for (int i = 0; i < num; i++) {
                if (triangles[i].contains(p)) {
                    p.y = triangles[i].getY(p);
                    break;
                }
            }
        }
        //描画----------------------------------------------------------------
        {
            //フィールド
            for (int i = 0; i < num; i++) {
                triangles[i].draw();
            }
            //キャラクタ
            if (isTrigger(KEY_C)) {
                ++dispCharaSw %= 2;
            }
            if (dispCharaSw == 0) {
                //キャラクタ(点)
                point(p, white, 80);
            }
            else {
                //キャラクタ(マッチ棒)
                VECTOR head = p + VECTOR(0, 0.4f, 0);
                segment(p, head, cyan, 30);
                point(head, cyan, 80);
            }
            //テキスト情報
            float size = 30;
            textSize(size);
            float colL = 10;//列の始まり
            float rowH = size + 10;//行の高さ
            int num = 0;//行番号
            text((let)"p.y:" + p.y, colL, ++num * rowH);
        }
    }
}
