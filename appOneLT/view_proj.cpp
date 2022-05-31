#include"view_proj.h"
#include"graphic.h"
#include"input.h"
#include"mathUtil.h"
#include"VECTOR.h"
//行列
extern MATRIX gWorld = { 0 }, gView = { 0 }, gProj = { 0 };
//カメラの位置決め用変数
float oAngleY = 20 * TO_RAD;
float oAngleX = 20 * TO_RAD;
float oRadius = 2;
float angleY = oAngleY;
float angleX = oAngleX;
float radius = oRadius;
int swCam = 0;
VECTOR camPos, lookat(0, 0, 0), up(0, 1, 0);
float speed = 0.01f;
let info = "Perspective";

void createProj()
{
    gProj.pers(45 * TO_RAD, width / height, 0.1f, 10);
}

void createOrthoProj()
{
    float w = radius * 0.73f;
    float h = w / width * height;
    gProj.ortho(-w, w, -h, h, -10, 10);
}

void updateView() 
{
    //初期状態に戻す
    if (isTrigger(KEY_P)) {
        swCam = 0;
        createProj();
        angleY = oAngleY;
        angleX = oAngleX;
        radius = oRadius;
        info = "Perspective";
    }
    //View切換
    if (isTrigger(KEY_SPACE)) {
        ++swCam %= 4;
        switch(swCam){
        case 0:
            createProj();
            angleY = oAngleY;
            angleX = oAngleX;
            //radius = oRadius;
            info = "Perspective";
            break;
        case 1:
            createOrthoProj();
            info = "Front";
            break;
        case 2:
            createOrthoProj();
            info = "Top";
            break;
        case 3:
            createOrthoProj();
            info = "Right";
            break;
        }
    }

    if (swCam == 0) {
        if (isPress(KEY_L))angleY += speed;
        if (isPress(KEY_J))angleY -= speed;
        if (isPress(KEY_I))angleX += speed;
        if (isPress(KEY_K))angleX -= speed;
        if (isPress(KEY_U))radius -= speed;
        if (isPress(KEY_O))radius += speed;
        if (radius < 1.0f)radius = 1.0f;
        if (angleX > 1.57f)angleX = 1.57f;
        if (angleX < -1.57f)angleX = -1.57f;
    }
    else if (swCam == 1) {
        angleY = 0;
        angleX = 0;
    }
    else if (swCam == 2) {
        angleY = 0;
        angleX = 1.57f;
    }
    else if (swCam == 3) {
        angleY = 1.57f;
        angleX = 0;
    }

    camPos.x = Sin(angleY) * Cos(angleX) * radius;
    camPos.y = Sin(angleX) * radius;
    camPos.z = Cos(angleY) * Cos(angleX) * radius;
    gView.camera(camPos, lookat, up);

    textSize(30);
    textMode(BOTTOM);
    text(info, width - 180, height);
}
