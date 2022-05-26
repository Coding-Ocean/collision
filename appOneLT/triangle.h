#pragma once
class TRIANGLE {
	VECTOR P[3];//頂点座標
	VECTOR S[3];//辺ベクトル
	VECTOR N;//法線ベクトル
	float D=0;//面の式ax+by+cz+d=0のd
	COLOR Color;
public:
	void create(const VECTOR& p0, const VECTOR& p1, const VECTOR& p2);
	bool contains(const VECTOR& p);
	float getY(const VECTOR& p);
	void draw();
};