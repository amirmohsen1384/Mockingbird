#ifndef USER_H
#define USER_H

#include "person.h"

class User : public Person
{
private:
    static QDir createRecord(const IDContainer &value);

public:
    static User loadFromRecord(const IDContainer &value);
    static IDs loadIDsFromRecord(const IDContainer &value);
    virtual void saveToRecord(const IDContainer &value) const override;
    static void saveIDsToRecord(const IDs &idList, const IDContainer &value);
};

#endif // USER_H
