#pragma once

#include "RgbColor.h"
#include "OpenGLSupport.h"

class Material
{
public:
    Material();
    Material(int a, GLfloat ns, GLfloat tr, RgbColor ka, RgbColor kd, RgbColor ke, RgbColor ks);
public:
    unsigned int newmtl;
    GLfloat Ns; // Shininess
    GLfloat Tr; //  and Transparency Values
    RgbColor Ka; // Ambient lighting in RgbColor
    RgbColor Kd; // Diffuse lighting in RgbColor
    RgbColor Ke; // Emissive lighting in RgbColor
    RgbColor Ks; // Specular lighting in RgbColor
};