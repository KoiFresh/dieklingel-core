#include <QCoreApplication>
#include "backend.h"
#include "socket.h"

/*
 *  Haupteinstiegspunk der anwendung
 */

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    dieklingel::socket::initialize(false); // false -> Protokolle und Logs werden nur über den Socket und nicht auf der Konsole ausgegeben
    dieklingel::backend backend;

    return a.exec();
}
