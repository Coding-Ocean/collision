#include"view_proj.h"
#include"VECTOR.h"
#include"COLOR.h"
#include"segment.h"

VECTOR axisX(1, 0, 0);
VECTOR axisY(0, 1, 0);
VECTOR axisZ(0, 0, 1);
VECTOR o(0, 0, 0);

void axis(const COLOR& color, float thickness) 
{
	segment(o, axisX, color, thickness);
	segment(o, axisY, color, thickness);
	segment(o, axisZ, color, thickness);
}