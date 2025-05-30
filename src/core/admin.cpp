#include "include/core/admin.h"
#include <QFile>

Admin Admin::loadFromRecord(const IDContainer &value)
{
    QFile file(MainFolder::getAdmin().absoluteFilePath(QString("%1.adf").arg(value)));
    if(!file.open(QFile::ReadOnly))
    {
        return Admin();
    }
    Admin value;
    QDataStream stream(&file);
    if((stream >> value).status() != QDataStream::Ok)
    {
        return Admin();
    }
    value.valid = true;
    return value;
}

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
