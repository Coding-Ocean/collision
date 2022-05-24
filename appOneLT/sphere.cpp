#include"view_proj.h"
#include"graphic.h"
#include"VECTOR.h"
#include"mathUtil.h"

static const int n = 8;//äpêî
static const int na = n * (n / 2 + 1);//í∏ì_êî
static float rs = 0.01f;
static VECTOR op[na];
void createSphere()
{
	float angle = 3.1415926f / (n / 2);
	for (int j = 0; j < n/2+1; j++) {
		float y = Cos(angle * j)*rs;
		float r = Sin(angle * j)*rs;
		for (int i = 0; i < n; i++) {
			op[n * j + i].set(Cos(angle*i) * r, y, Sin(angle*i) * r);
		}
	}
}
void sphere(const VECTOR& t, const COLOR& col)
{
    MATRIX world;
    world.identity();
    world.mulTranslate(t);
    //ÇªÇµÇƒç¿ïWïœä∑
    VECTOR p[na];
    for (int i = 0; i < na; i++) {
        p[i] = world * op[i];
        p[i] = gView * p[i];
        p[i] = gProj * p[i];
    }
    //ï`âÊ
    for (int i = 0; i < na-n; i++) {
        int a = i;
        int b = i + n;
        int c = (i + 1) % n == 0 ? i + 1 - n : i + 1;
        int d = (b + 1) % n == 0 ? b + 1 - n : b + 1;
        triangle3D(p[a], p[b], p[c], col, col, col);
        triangle3D(p[c], p[b], p[d], col, col, col);
    }
}
