#ifndef USER_H
#define USER_H

#include "person.h"

class User : public Person
{
private:
    static QDir createRecord(const IDContainer &value);

public:
    enum UserRole
    {
        KeyRole = Qt::UserRole,
        NameRole = Qt::DisplayRole,
        FirstNameRole = Qt::UserRole + 1,
        LastNameRole = Qt::UserRole + 2,
        UserNameRole = Qt::UserRole + 3,
        PasswordRole = Qt::UserRole + 4,
        ModelRole = Qt::UserRole + 5,
        KeyListRole = Qt::UserRole + 6,
        SavedRole = Qt::UserRole + 7,
        LikedRole = Qt::UserRole + 8
    };
    static User loadFromRecord(const IDContainer &value);
    static IDs loadIDsFromRecord(const IDContainer &value);
    virtual void saveToRecord(const IDContainer &value) const override;
    static void saveIDsToRecord(const IDs &idList, const IDContainer &value);

    static void createAccount(User &user);

    const IDs& getSpecialKeys() const;
    IDContainer getLikedPlaylist() const;
    IDContainer getSavedPlaylist() const;

    friend QDataStream& operator<<(QDataStream &stream, const User &data);
    friend QDataStream& operator>>(QDataStream &stream, User &data);

private:
    IDs specialKeys;
};

QDataStream& operator<<(QDataStream &stream, const User &data);
QDataStream& operator>>(QDataStream &stream, User &data);

#endif // USER_H
