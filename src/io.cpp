#include "io.h"
#include <QSound>
#include <QDebug>
#ifdef WIRINGPI
    #include <wiringPi.h>
#endif

#define MOVEMENT_PIN 4

dieklingel::Io *dieklingel::Io::s_instance_ = nullptr;
bool dieklingel::Io::s_bRisingEdgeMovement_ = false;

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
    s_instance_ = new Io();
    //s_iteration_ = new QTimer();
    //s_iteration_->setInterval(500);
    //connect(m_iteration, &QTimer::timeout, m_instance, Io::m_iterate);
    // cbot m_iteration->start();
}

dieklingel::Io *dieklingel::Io::s_instance()
{
    if(nullptr == s_instance_) 
    {
#ifdef WIRINGPI
        wiringPiSetup();
        pinMode(MOVEMENT_PIN, INPUT);
#endif

#ifdef WIRINGPI
        qInfo() << "[INFO][io.cpp, init()] \r\n\t Compiled with wiring pi";
#else
        qInfo() << "[INFO][io.cpp, init()] \r\n\t Compiled without wiring pi";
#endif
        s_instance_ = new Io();
    }
    return s_instance_;
}

void dieklingel::Io::s_iterate()
{
    // check for movement
#ifdef WIRINGPI
    bool bState = digitalRead(MOVEMENT_PIN);
    if(bState)
    {
    #ifdef DEBUG
        qDebug() << "[DEBUG]][io.cpp, init()] \r\n\t movement detected";
    #endif
        emit dieklingel::Io::s_instance()->movementDetected(bState > s_bRisingEdgeMovement_);
        //emit movementDetected((state > risingEdgeMovement));
    }
    s_bRisingEdgeMovement_ = bState;
#endif
}