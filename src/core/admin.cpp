#include "include/core/admin.h"
#include <QFile>

Admin Admin::loadFromRecord(const IDContainer &value)
{
    QFile file(MainFolder::getAdmin().absoluteFilePath(QString("%1.adf").arg(value)));
    if(!file.open(QFile::ReadOnly))
    {
        return Admin();
    }
    Admin target;
    QDataStream stream(&file);
    if((stream >> target).status() != QDataStream::Ok)
    {
        return Admin();
    }
    target.valid = true;
    return target;
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
