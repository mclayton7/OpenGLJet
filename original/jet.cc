// OpenGL Jet program.
// ECE8893, Georgia Tech, Fall 2012
// Author: Mac Clayton, 2012

#include <iostream>
#include <math.h>

#ifdef LINUX
//Linux Headers
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#ifdef OSX
// MAC Headers
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif

#ifdef WINDOWS
//Windows Headers
#include <Windows.h>
#include <gl/GL.h>
#include <gl/glut.h>
#endif

#include <fstream>
#include <vector>
#include <string>

using namespace std;

static float xpos = 0, ypos = 0, zpos = 0;


class RGBColor
{
public:
    RGBColor(GLfloat x, GLfloat y, GLfloat z) : r(x), g(y), b(z) {}
public:
    GLfloat r, g, b;
};

class Vertex
{
public:
    //Constructors
    Vertex(GLfloat a, GLfloat b, GLfloat c) : x(a), y(b), z(c) {}
    Vertex() : x(-1), y(-1), z(-1) {}
public:
    GLfloat x, y, z;      // X, Y, and Z Vertices
};

class Normal : public Vertex
{
public:
    // Constructors
    Normal(GLfloat a, GLfloat b, GLfloat c) : Vertex(a, b, c) {}
public:
    // No necessary member variables
};

class vTexture : public Vertex
{
public:
    // Constructor:
    vTexture() : Vertex() {}
    vTexture(GLfloat a, GLfloat b, GLfloat c) : Vertex(a, b, c) {}
public:
    // No needed member variables
};

class Face
{
public:
    Face(int mtl, Vertex a, vTexture c, Vertex b, bool texture = true) : material(mtl), v(a), t(c), n(b), hasTexture(texture) {}
    Face(int mtl, Vertex a, Vertex b, bool texture = false) : material(mtl), v(a), n(b), hasTexture(texture) {}

public:
    int material;
    bool hasTexture;
    Vertex v, n;     // First, Second, and Third Vertices of Face
    vTexture t;
    // Normal x, y, z;     // Normal
};

class Material
{
public:
    Material()
        : newmtl(0), Ns(0), Tr(0), Ka(RGBColor(0,0,0)), Kd(RGBColor(0,0,0)), Ke(RGBColor(0,0,0)), Ks(RGBColor(0,0,0)) {}
    Material(int a, GLfloat ns, GLfloat tr, RGBColor ka, RGBColor kd, RGBColor ke, RGBColor ks)
        : newmtl(a), Ns(ns), Tr(tr), Ka(ka), Kd(kd), Ke(ke), Ks(ks) {}
public:
    unsigned int newmtl;
    GLfloat Ns, Tr;     // Shininess and Transparency Values
    RGBColor Ka;             // Ambient lighting in RGBColor
    RGBColor Kd;             // Diffuse lighting in RGBColor
    RGBColor Ke;             // Emissive lighting in RGBColor
    RGBColor Ks;             // Specular lighting in RGBColor
};


// Global Variables:
GLfloat updateRate = 140.0;          // Change this later (later changed to 20)
vector<Vertex*> vertex;             // Vector of vertecies
vector<Normal*> normal;             // Vector of normal vectors
vector<vTexture*> vTextures;        // Vector of Textures
vector<Face*> faces;                // Vector of faces
vector<Material*> materials;        // Vector of materials

bool xstat, ystat, zstat = false;   // Keep track of rotation status
float xangle, yangle, zangle = 0;   // Initalize rotation angles to zero
float scale = 1.2;                  // Start scaling factor at 1.2


// code to read the obj file
void Read()
{
    // ifstream ifs("testJet.obj");
    ifstream ifs("jet.obj");
    if (!ifs) return;
    int mtl = 0;
    while(ifs)
    {
        string header;
        ifs >> header;

        if (header == string("mtllib"))
        {
            string mtllib;
            ifs >> mtllib;
        }
        // If the header is for a Vertex:
        else if (header == string("v"))
        {
            float v[3];
            ifs >> v[0] >> v[1] >> v[2];
            vertex.push_back(new Vertex(v[0], v[1], v[2]));
        }
        // If the header is for a Normal Vertex
        else if (header == string("vn"))
        {
            float n[3];
            ifs >> n[0] >> n[1] >> n[2];
            normal.push_back(new Normal(n[0], n[1], n[2]));
        }
        else if (header == string("vt"))
        {
            float vt[3];
            ifs >> vt[0] >> vt[1] >> vt[2];
            vTextures.push_back(new vTexture(vt[0], vt[1], vt[2]));

        }
        else if (header == string("usemtl"))
        {
            ifs >> mtl;
        }
        // If the header is for a Face:
        else if (header == string("f"))
        {
            // Push the face coordinates into a, b, and c strings
            string a,b,c;
            ifs >> a >> b >> c;
            bool type = true;           // 0 for xxx//xxx
                                        // 1 for xxx/xxx/xxx
            GLfloat x, y, z = 0;        // Initialize x, y, and z
            // Assume Only Triangles:
            for(int i = 0; i < 3; ++i)
            {
                string temp = "";
                switch (i)
                {
                    case 0:
                    temp = a;
                    break;
                    case 1:
                    temp = b;
                    break;
                    case 2:
                    temp = c;
                    break;
                }
                x, y, z = 0;
                int part = 0;
                string face;
                for(int j = 0; j < temp.length(); ++j)
                {
                    // If the character isn't a slash, add it to our temp string
                    if(temp[j] != '/')
                    {
                        face += temp[j];
                    }
                    // If the character is a slash, store the number
                    if(temp[j] == '/')
                    {
                        // If there's no texture coordinate set type to zero:
                        if(temp[j + 1] == '/') type = false;
                        // Parse the first number array into an integer
                        if((part == 0) && type)
                        {
                            x = atoi(face.c_str());     // Convert the string to integer
                            face = "";                  // Reset the face string
                            ++part;                     // Increment part
                        }
                        else if((part == 1) && type)
                        {
                            y = atoi(face.c_str());
                            face = "";
                            ++part;
                        }
                        // If the texture vertex is missing, parse the first part:
                        else if((part == 0) && !type)
                        {
                            x = atoi(face.c_str());
                            y = -1;
                            face = "";
                            ++part;
                        }
                    }
                    if(j == (temp.length() - 1))
                    {
                        z = atoi(face.c_str());
                    }
                }// End number array for loop
            if(type)  faces.push_back(new Face(mtl, *(vertex[x-1]), *(vTextures[y-1]), *(normal[z-1])));
            if(!type) faces.push_back(new Face(mtl, *(vertex[x-1]), *(normal[z-1]), false));
            }
        }// End Faces
    }
}


// Code to read material file
void ReadMtl()
{
    ifstream ifs("jet.mtl");
    if (!ifs) return;
    cout.precision(4);
    Material temp = Material();
    while(ifs)
    {
        string header;
        ifs >> header;
        if (header == string("newmtl"))
        {
            int n;
            ifs >> n;
            temp.newmtl = n;
        }
        else if (header == string("Ns"))
        {
            float Ns;
            ifs >> Ns;
            temp.Ns = Ns;
        }
        else if (header == string("Tr"))
        {
            float Tr;
            ifs >> Tr;
            temp.Tr = Tr;
        }
        else if (header == string("Ka"))
        {
            float Ka[3];
            ifs >> Ka[0] >> Ka[1] >> Ka[2];
            temp.Ka = RGBColor(Ka[0], Ka[1], Ka[2]);
        }
        else if (header == string("Kd"))
        {
            float Kd[3];
            ifs >> Kd[0] >> Kd[1] >> Kd[2];
            temp.Kd = RGBColor(Kd[0], Kd[1], Kd[2]);
        }
        else if (header == string("Ks"))
        {
            float Ks[3];
            ifs >> Ks[0] >> Ks[1] >> Ks[2];
            temp.Ks = RGBColor(Ks[0], Ks[1], Ks[2]);
        }
        else if (header == string("Ke"))
        {
            float Ke[3];
            ifs >> Ke[0] >> Ke[1] >> Ke[2];
            temp.Ke = RGBColor(Ke[0], Ke[1], Ke[2]);
            // Since Ke is the last material value, we can push temp onto the vector
            materials.push_back(new Material(temp));
        }
    }
}

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
    cout << "Use X, Y, and Z for Rotation" << endl;
    cout << "s and S for zoom, and r to reset the angles" << endl;
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
    Read();
    ReadMtl();
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

