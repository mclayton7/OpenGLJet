#include "ObjReader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <QString>
#include <QFile>

using namespace std;

ObjReader::ObjReader()
{
}

ObjReader::~ObjReader()
{
}

QVector<Face*> ObjReader::readFile(const QString file)
{
    QVector<Vertex*> vertex;
    QVector<Normal*> normal;
    QVector<vTexture*> vTextures;
	QVector<Face*> faces;

    ifstream ifs(file.toLatin1());
    if (!ifs)
    {
    	return faces;
    }
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
    return faces;
}