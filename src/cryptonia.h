#ifndef CRYPTONIA_H
#define CRYPTONIA_H

#include <QObject>
#include <iostream>
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>
#include <crypto++/hex.h>
#include <crypto++/sha.h>

/*
 *  cryptonia.h
 *  Ver- und Entschschlüsseln von Strings
 */

class cryptonia : public QObject
{
    Q_OBJECT
public:
    static QString Encrypt(QString text, QString key, QString iv);
    static QString Decrypt(QString text, QString key, QString iv);
    static QString Normalize(QString text, int length = 32);
    static QString RandomIV(int length);
private:
};

#endif // CRYPTONIA_H
