#pragma once

#include <QVector>
#include "Vertex.h"
#include "Face.h"

class ObjReader
{
public:
	ObjReader();
	~ObjReader();

	QVector<Face*> readFile(const QString file);
};