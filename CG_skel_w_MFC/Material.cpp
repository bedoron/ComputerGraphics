#include "stdafx.h"
#include "Material.h"


Material::Material(std::string val):name(val),_ns(0),_kambiant(vec3(0)),_kspecular(vec3(0)),_kdiffuze(vec3(0))
{
}


Material::~Material(void)
{
}
