#include "include/core/admin.h"
#include <QFile>

void Admin::saveToRecord(const IDContainer &value) const
{
    QFile file(MainFolder::getAdmin().absoluteFilePath(QString("%1.adf").arg(value)));
    if(!file.open(QFile::WriteOnly))
    {
        return;
    }
    QDataStream stream(&file);
    if((stream << *this).status() != QDataStream::Ok)
    {
        return;
    }
}
