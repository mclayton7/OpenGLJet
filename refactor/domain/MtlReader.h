#pragma once

#include <QVector>
#include "Material.h"

class MtlReader
{
public:
	MtlReader();
	~MtlReader();

	QVector<Material*> readFile(const QString& file);
};