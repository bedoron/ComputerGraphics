#include "stdafx.h"
#include "VNormal.h"


VNormal::VNormal(float i,float j,float k):_i(i),_j(j),_k(k)
{
}


VNormal::~VNormal(void)
{
}
float VNormal::getI()
{
	return _i;
}
float VNormal::getJ()
{
	return _j;
}
float VNormal::getK()
{
	return _k;
}