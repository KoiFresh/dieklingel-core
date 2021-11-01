#ifndef DUPSERVER_H
#define DUPSERVER_H

#include <QObject>

class dupserver : public QObject
{
    Q_OBJECT
public:
    explicit dupserver(QObject *parent = nullptr);

signals:

};

#endif // DUPSERVER_H
