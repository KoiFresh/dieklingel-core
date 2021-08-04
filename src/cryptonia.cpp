#include "cryptonia.h"
#include <QDebug>
#include <QRandomGenerator>
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>
#include <crypto++/hex.h>
#include <crypto++/sha.h>
#include <QTextCodec>
#include <QDataStream>

QString cryptonia::Encrypt(QString text, QString key, QString iv)
{
    using namespace CryptoPP;
    std::string ciphertext;
    std::string plaintext = text.toStdString();
    QByteArray qbak = key.toUtf8();
    byte* KEY = reinterpret_cast<byte*>(qbak.data());
    QByteArray qbai = iv.toUtf8();
    byte* IV = reinterpret_cast<byte*>(qbai.data());

    CBC_Mode<AES>::Encryption encryptor(KEY,32, IV);

    StringSource( plaintext, true, new StreamTransformationFilter( encryptor, new HexEncoder(new StringSink( ciphertext ) ) ) );
    return  QString::fromStdString(ciphertext).toLower();
}


QString cryptonia::Decrypt(QString text, QString key, QString iv)
{
    using namespace CryptoPP;
    std::string plaintext;
    std::string result;
    QByteArray qbak = key.toLocal8Bit();
    byte* KEY = reinterpret_cast<byte*>(qbak.data());
    QByteArray qbai = iv.toLocal8Bit();
    byte* IV = reinterpret_cast<byte*>(qbai.data());

    QByteArray cipher = QByteArray::fromHex(text.toUtf8());
    byte  ciphers[cipher.length()];
    for(int i = 0; i < cipher.length() ; i++)
    {
        ciphers[i] = ((int)cipher[i] & 255);
    }
    plaintext.append((const char*)ciphers, sizeof (ciphers));

    CBC_Mode<AES>::Decryption decryptor(KEY, 32, IV);

    CryptoPP::StreamTransformationFilter stfEncryptor(decryptor, new CryptoPP::StringSink( result));
    stfEncryptor.Put( reinterpret_cast<const byte*>( plaintext.c_str() ), plaintext.length());
    stfEncryptor.MessageEnd();
    return QString::fromStdString(result);
}

QString cryptonia::Normalize(QString text, int length)
{
    QString result = text;
    while (result.length() < length)
    {
        result += text;
    }
    return  result.left(length);
}

QString cryptonia::RandomIV(int length)
{
    QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString result = "";
    for(int i = 0; i < length; i++)
    {
        int random = QRandomGenerator::global()->bounded(chars.length() - 1);
        result += chars[random];
    }
    return result;
}
