#include "Material.h"

Material::Material()
	: newmtl(0)
	, Ns(0)
	, Tr(0)
	, Ka(RgbColor(0,0,0))
	, Kd(RgbColor(0,0,0))
	, Ke(RgbColor(0,0,0))
	, Ks(RgbColor(0,0,0))
{
}

Material::Material(int a, GLfloat ns, GLfloat tr, RgbColor ka, RgbColor kd, RgbColor ke, RgbColor ks)
	: newmtl(a)
	, Ns(ns)
	, Tr(tr)
	, Ka(ka)
	, Kd(kd)
	, Ke(ke)
	, Ks(ks)
{
}
