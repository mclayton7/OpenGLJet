#include "MtlReader.h"

#include <QVector>
#include <QString>
#include <QFile>
#include <QTextStream>

MtlReader::MtlReader()
{
}

MtlReader::~MtlReader()
{
}

QVector<Material*> MtlReader::readFile(const QString& file)
{
	QVector<Material*> materials;
    QFile inputFile(file);
    if(!inputFile.open(QIODevice::ReadOnly))
    {
        return materials;
    }
    QTextStream in(&inputFile);
    Material temp = Material();
    while(!in.atEnd())
    {
        QString header = in.readLine();
        if (header.contains("newmtl"))
        {
            temp.newmtl = header.split(" ",QString::SkipEmptyParts).last().toInt();
        }
        else if (header.contains("Ns"))
        {
            temp.Ns = header.split(" ",QString::SkipEmptyParts).last().toFloat();
        }
        else if (header.contains("Tr"))
        {
            temp.Tr = header.split(" ",QString::SkipEmptyParts).last().toFloat();
        }
        else if (header.contains("Ka"))
        {
            QVector<float> Ka(3);
            QStringList line = header.split(" ",QString::SkipEmptyParts);
            for (int i = 1; i < line.length(); ++i)
            {
                Ka.append(line.at(i).toFloat());
            }
            temp.Ka = RgbColor(Ka[0], Ka[1], Ka[2]);
        }
        else if (header.contains("Kd"))
        {
            QVector<float> Kd(3);
            QStringList line = header.split(" ",QString::SkipEmptyParts);
            for (int i = 1; i < line.length(); ++i)
            {
                Kd.append(line[i].toFloat());
            }
            temp.Kd = RgbColor(Kd[0], Kd[1], Kd[2]);
        }
        else if (header.contains("Ks"))
        {
            QVector<float> Ks(3);
            QStringList line = header.split(" ",QString::SkipEmptyParts);
            for (int i = 1; i < line.length(); ++i)
            {
                Ks.append(line[i].toFloat());
            }
            temp.Ks = RgbColor(Ks[0], Ks[1], Ks[2]);
        }
        else if (header.contains("Ke"))
        {
            QVector<float> Ke(3);
            QStringList line = header.split(" ",QString::SkipEmptyParts);
            for (int i = 1; i < line.length(); ++i)
            {
                Ke.append(line[i].toFloat());
            }
            temp.Ke = RgbColor(Ke[0], Ke[1], Ke[2]);
            // Since Ke is the last material value, we can push temp onto the vector
            materials.push_back(new Material(temp));
        }
    }
    inputFile.close();
    return materials;
}