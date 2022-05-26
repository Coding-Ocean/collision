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
    //フィールドを構成する三角ポリゴン
    const int num = 8;
    TRIANGLE triangles[num];
    VECTOR center;
    center.set(-1.0f, 0.7f, 0.3f);
    triangles[0].create(center, VECTOR(-2, 0, -1), VECTOR(-2, 0, 1));
    triangles[1].create(center, VECTOR(-2, 0, 1), VECTOR(0, 0, 1));
    triangles[2].create(center, VECTOR(0, 0, 1), VECTOR(0, 0, -1));
    triangles[3].create(center, VECTOR(0, 0, -1), VECTOR(-2, 0, -1));
    center.set(1.3f, -0.5f, -0.5f);
    triangles[4].create(center, VECTOR(0, 0, -1), VECTOR(0, 0, 1));
    triangles[5].create(center, VECTOR(0, 0, 1), VECTOR(2, 0, 1));
    triangles[6].create(center, VECTOR(2, 0, 1), VECTOR(2, 0, -1));
    triangles[7].create(center, VECTOR(2, 0, -1), VECTOR(0, 0, -1));
    //-----------------------------------------------------------------------
    //点の座標p
    VECTOR p;
    float angle = 0;
    //その他------------------------------------------------------------------
    //色
    COLOR red(255, 0, 0);
    COLOR green(0, 255, 0);
    COLOR cyan(0, 255, 255);
    //プロジェクション行列を作っておく
    createProj();
    initDeltaTime();
    //メインループ-------------------------------------------------------------
    while (notQuit) {
        setDeltaTime();
        clear();
        //カメラ行列を更新
        updateView();
        //点を楕円運動させる----------------------------------------------------
        {
            angle += 0.6f * delta;
            p.x = Cos(angle) * 1.8f;
            p.y = 0.0f;//とりあえずゼロ
            p.z = Sin(angle) * 0.8f;
        }
        //三角形が点(p.x p.z)を含んでいたらp.yを取得する--------------------------
        {
            for (int i = 0; i < num; i++) {
                if (triangles[i].contains(p)) {
                    p.y = triangles[i].getY(p);
                }
            }
        }
        //描画----------------------------------------------------------------
        {
            //フィールド
            for (int i = 0; i < num; i++) {
                triangles[i].draw();
            }
            //ポイント
            VECTOR head = p + VECTOR(0, 0.4f, 0);
            segment(p, head, cyan, 40);
            point(head, cyan, 80);
            point(p, red, 60);
            //point(VECTOR(p.x, 0.8f, p.z), green, 60);
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
