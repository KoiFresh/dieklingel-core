#ifndef IO_H
#define IO_H

#include <opencv2/opencv.hpp>
#include <QObject>
#include <QTimer>

namespace dieklingel {
    class Io : public QObject
    {
        Q_OBJECT
    public:
        static void init();
        static Io *getInstance();
    signals:
        void movementDetected(bool risingEdge = false);
    private:
        Io();
        static Io *m_instance;
        static QTimer *m_iteration;
        static bool risingEdgeMovement;
    private slots:
        static void m_iterate();
    };
}


#endif // IO_H
