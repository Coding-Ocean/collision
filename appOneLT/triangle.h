#pragma once
class TRIANGLE {
	VECTOR P[3];//���_���W
	VECTOR S[3];//�Ӄx�N�g��
	VECTOR N;//�@���x�N�g��
	float D=0;//�ʂ̎�ax+by+cz+d=0��d
	COLOR Color;
public:
	void create(const VECTOR& p0, const VECTOR& p1, const VECTOR& p2);
	bool contains(const VECTOR& p);
	float getY(const VECTOR& p);
	void draw();
};