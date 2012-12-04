#pragma once
class VNormal
{
private:
	float _i,_j,_k;
public:
	VNormal(float i,float j,float k);
	~VNormal(void);
	float getI();
	float getJ();
	float getK();

};

