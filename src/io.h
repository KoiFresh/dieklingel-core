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
        static Io *s_instance();
        static void s_iterate();
    signals:
        void movementDetected(bool risingEdge = false);
    private:
        Io();
        static Io *s_instance_;
        static QTimer *s_iteration_;
        static bool s_bRisingEdgeMovement_;
    private slots:
        //static void m_iterate();
    };
}


#endif // IO_H
