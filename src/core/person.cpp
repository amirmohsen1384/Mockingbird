#include "include/core/person.h"
#include <QRegularExpression>

QString Person::getFirstName() const
{
    return firstName;
}

QString Person::getUserName() const
{
    return userName;
}

QString Person::getLastName() const
{
    return lastName;
}

QString Person::getPassword() const
{
    return password;
}

QString Person::getFullName() const
{
    return QString("%1 %2").arg(firstName).arg(lastName);
}

bool Person::setFirstName(const QString &value)
{
    // Matches only with values that has no non-letter characters.
    QRegularExpression pattern("[a-zA-Z ]{1,}");

    // Checks if the given values matches with the regex.
    if(pattern.match(value).hasMatch())
    {
        firstName = value;
        return true;
    }
    else
    {
        return false;
    }
}

bool Person::setLastName(const QString &value)
{
    // Matches only with values that has no non-letter characters.
    QRegularExpression pattern("[a-zA-Z ]{1,}");

    // Checks if the given values matches with the regex.
    if(pattern.match(value).hasMatch())
    {
        lastName = value;
        return true;
    }
    else
    {
        return false;
    }
}

bool Person::setUserName(const QString &value)
{
    // Matches only with values with at least 6 characters.
    QRegularExpression pattern(".{6,}");

    // Checks if the given values matches with the regex.
    if(pattern.match(value).hasMatch())
    {
        userName = value;
        return true;
    }
    else
    {
        return false;
    }
}

bool Person::setPassword(const QString &value)
{
    /* Matches only with values with
     * at least 8 characters
     * one uppercase letter
     * one lowercase letter
     * one non-character letter
     * one number
    */
    QRegularExpression pattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[\\W_])[A-Za-z\\d\\W_]{8,}$");

    // Checks if the given values matches with the regex.
    if(pattern.match(value).hasMatch())
    {
        password = value;
        return true;
    }
    else
    {
        return false;
    }
}

QDataStream& operator<<(QDataStream &stream, const Person &data)
{
    stream << data.firstName << data.lastName;
    stream << data.userName << data.password;
    return stream;
}

QDataStream& operator>>(QDataStream &stream, Person &data)
{
    stream >> data.firstName >> data.lastName;
    stream >> data.userName >> data.password;
    return stream;
}
