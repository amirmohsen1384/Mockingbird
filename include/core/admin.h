#ifndef ADMIN_H
#define ADMIN_H

#include "person.h"

class Admin : public Person
{
public:
    static Admin loadFromRecord(const IDContainer &value);
    void saveToRecord(const IDContainer &value) const override;
};

#endif // ADMIN_H
