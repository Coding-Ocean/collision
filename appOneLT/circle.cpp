#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"
#include"mathUtil.h"
#include"QUATERNION.h"

static const int num = 36;
static VECTOR op[num];

void createCircle()
{
    //上を向いた円
    float deg = 360.0f / num;
    for (int i = 0; i < num; i++)
    {
        float rad = deg * i * TO_RAD;
        op[i].set(Cos(rad), 0, -Sin(rad));
    }
}
void circleEu(VECTOR n, const COLOR& c)
{
    VECTOR r(0, 0, 0);
    //円の頂点をｘ軸回転させる角度-----------------------------
    n.normalize();
    //VECTOR(0,1,0)とｎの内積はcosΘ、この場合cosΘ=n.yとなる
    r.x = Acos(n.y);
    //円の頂点をｙ軸回転させる角度------------------------------
    n.y = 0;//２次元ベクトル化。（ｘ、ｚ平面で考える）
    n.normalize();
    //底辺はdot(VECTOR(0,0,1),n)=n.z;
    //高さはcross(VECTOR(0,0,1),n)=n.x;
    r.y = Atan2(n.x, n.z);

    MATRIX world;
    world.identity();
    world.mulRotateYXZ(r);
    //そして座標変換
    VECTOR p[num];
    for (int i = 0; i < num; i++) {
        p[i] = world * op[i];
        p[i] = gView * p[i];
        p[i] = gProj * p[i];
    }
    //描画
    for (int i = 0; i < num-2; i++) {
        triangle3D(p[0], p[i + 1], p[i + 2], c, c, c);
    }
}
//クォータニオンでｆ方向に向けて描画
void circleQt(VECTOR n, const COLOR& c)
{
    //オリジナルの円の法線は上を向いている
    VECTOR on(0, 1, 0);
    //onをnに向けるための回転軸axisを求める
    n.normalize();
    VECTOR axis = cross(on, n);
    axis.normalize();
    //回転角度を求める
    float theta = Acos(dot(on, n));
    //情報表示
    text((let)"axis(" + 
        axis.x + "," + 
        axis.y + "," + 
        axis.z + ")" +
        " thata:" + theta, 1300, 35);
    //クォータニオンからワールド座標変換行列を作る
    QUATERNION q(axis, theta);
    MATRIX world;
    world.identity();
    world.mulRotateQuaternion(q);
    //そして座標変換
    VECTOR p[num];
    for (int i = 0; i < num; i++) {
        p[i] = world * op[i];
        p[i] = gView * p[i];
        p[i] = gProj * p[i];
    }
    //描画
    for (int i = 0; i < num - 2; i++) {
        triangle3D(p[0], p[i + 1], p[i + 2], c, c, c);
    }
}

