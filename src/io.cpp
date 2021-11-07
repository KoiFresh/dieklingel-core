#include "io.h"
#include <QSound>
#include <QDebug>
#ifdef WIRINGPI
    #include <wiringPi.h>
#endif

#define MOVEMENT_PIN 4

dieklingel::Io *dieklingel::Io::m_instance = nullptr;
QTimer *dieklingel::Io::m_iteration = nullptr;

dieklingel::Io::Io()
{

}

void dieklingel::Io::init()
{
#ifdef WIRINGPI
    wiringPiSetup();
    pinMode(MOVEMENT_PIN, INPUT);
#endif

#ifdef WIRINGPI
    qDebug() << "[INFO][io.cpp, init()] \r\n\t Compiled with wiring pi";
#else
    qDebug() << "[INFO][io.cpp, init()] \r\n\t Compiled without wiring pi";
#endif
    m_instance = new Io();
    m_iteration = new QTimer();
    m_iteration->setInterval(1000);
    connect(m_iteration, &QTimer::timeout, m_instance, Io::m_iterate);
}

dieklingel::Io *dieklingel::Io::getInstance()
{
    return m_instance;
}

void dieklingel::Io::m_iterate()
{
    // check for movement
#ifdef WIRINGPI
    bool state = digitalRead(MOVEMENT_PIN);
    if(state)
    {
        emit movementDetected((state > risingEdgeMovement));
    }
    risingEdgeMovement = state;
#endif
}
