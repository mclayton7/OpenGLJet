// OpenGL Jet program.
// ECE8893, Georgia Tech, Fall 2012
// Author: Mac Clayton, 2012

#include "OpenGLSupport.h"
#include "RgbColor.h"
#include "Vertex.h"
#include "ObjReader.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include "Face.h"
#include "Material.h"
#include "MtlReader.h"

// Global Variables:
GLfloat updateRate = 140.0;          // Change this later (later changed to 20)
QVector<Face*> faces;                // Vector of faces
QVector<Material*> materials;        // Vector of materials

bool xstat, ystat, zstat = false;   // Keep track of rotation status
float xangle, yangle, zangle = 0;   // Initalize rotation angles to zero
float scale = 1.2;                  // Start scaling factor at 1.2

void setMaterial(int materialId)
{
    GLfloat specular[4];
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat emmisive[4];
    GLfloat shiny;

    // Ugly but works
    specular[0] = materials[materialId]->Ks.r;
    specular[1] = materials[materialId]->Ks.g;
    specular[2] = materials[materialId]->Ks.b;
    specular[3] = 1 - materials[materialId]->Tr;
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    ambient[0] = materials[materialId]->Ka.r;
    ambient[1] = materials[materialId]->Ka.g;
    ambient[2] = materials[materialId]->Ka.b;
    ambient[3] = 1 - materials[materialId]->Tr;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    diffuse[0] = materials[materialId]->Kd.r;
    diffuse[1] = materials[materialId]->Kd.g;
    diffuse[2] = materials[materialId]->Kd.b;
    diffuse[3] = 1 - materials[materialId]->Tr;
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    emmisive[0] = materials[materialId]->Ke.r;
    emmisive[1] = materials[materialId]->Ke.g;
    emmisive[2] = materials[materialId]->Ke.b;
    emmisive[3] = 1 - materials[materialId]->Tr;
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emmisive);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &materials[materialId]->Ns);
}

void drawModel()
{
    // Your code to draw the model here
    glBegin(GL_TRIANGLES);
    for(unsigned i = 0; i < faces.size(); ++i)
    {
        // Set the Material:
        setMaterial(faces[i]->material);
        // If the texture exists:
        if(faces[i]->hasTexture)
        {
            glTexCoord3f(faces[i]->t.x,
                         faces[i]->t.y,
                         faces[i]->t.z);
        }
        // Draw normal vectors and vertices
        glNormal3f( faces[i]->n.x,
                    faces[i]->n.y,
                    faces[i]->n.z);
        glVertex3f( faces[i]->v.x,
                    faces[i]->v.y,
                    faces[i]->v.z);
    }
    glEnd();
}


void init(void)
{
    // Called from main
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}



void timer(int)
{
    // Calculate Rotation angles:
    if(xstat)
    {
        xangle += 0.5;
        if(xangle > 360) xangle -= 360;
    }
    if(xstat)
    {
        xangle += 0.5;
        if(xangle > 360) xangle -= 360;
    }
    if(ystat)
    {
        yangle += 0.5;
        if(yangle > 360) yangle -= 360;
    }
    if(zstat)
    {
        zangle += 0.5;
        if(zangle > 360) zangle -= 360;
    }

    // Then tell glut to redisplay
    glutPostRedisplay();
    // And reset the timer
    glutTimerFunc(1000.0 / updateRate, timer, 0);
}

void display(void)
{
    // glClearColor(0.0, 0.0, 0.0, 1.0); // black background
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 15.0, -25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // rotations and scaling here
    glPushMatrix();
    glRotatef(xangle, 1, 0, 0);        // X axis rotation
    glRotatef(yangle, 0, 1, 0);        // Y axis rotation
    glRotatef(zangle, 0, 0, 1);        // Z axis rotation
    glScalef(scale, scale, scale);  // Set scaling factor

    // Draw th emodel
    drawModel();
    // Swap the double buffers
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}


void keyboard (unsigned char key, int x, int y)
{

    switch (key)
    {
        case 27:
            exit(0);            // Exit on ESC key
        break;
        case 'q':
            exit(0);            // Exit on 'q'
        break;
        case 'x':
            xstat = !xstat;     // Toggle X rotation
        break;
        case 'y':
            ystat = !ystat;     // Toggle Y rotation
        break;
        case 'z':
            zstat = !zstat;     // Toggle Z rotation
        break;
        case 's':
            scale -= 0.1;       // Zoom out
        break;
        case 'S':
            scale += 0.1;       // Zoom in
        break;
        case 'r':               // Reset the angles
            xangle = 0;
            yangle = 0;
            zangle = 0;
        break;
    }
}


int main(int argc, char* argv[])
{
    std::cout << "Use X, Y, and Z for Rotation" << std::endl;
    std::cout << "s and S for zoom, and r to reset the angles" << std::endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    if(argc == 2)
    {
        glutInitWindowSize(atol(argv[1]), atol(argv[1]));
    }
    else
    {
        glutInitWindowSize(500, 500);
    }
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Jet Display");
    ObjReader facesReader;
    printf("Reading face");
    faces = facesReader.readFile("jet.obj");
    MtlReader mtlReader;
    printf("Reading mtl");
    materials = mtlReader.readFile("jet.mtl");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc (keyboard);
    glutReshapeFunc(reshape);
    // Compute the update rate here...
    glutTimerFunc(1000.0 / updateRate, timer, 0);
    glutMainLoop();
    return 0;
}
