#ifndef FFMPEG_H
#define FFMPEG_H

#include <QObject>
#include <QProcess>
#include <opencv2/opencv.hpp>

namespace ffmpeg
{

    class Stream : public QObject
    {
        Q_OBJECT
    public:
        Stream();
        ~Stream();
        void startCamera(int source = 0, int sink = 1);
        void splitCamera(int source,QList<int> sinks);
        void stopCamera();
        QList<int> getCameras();
        int getNextCamera();
        bool waitForCameraReady();
    public slots:
        void startStream(QString url, int framerate, QString resolution, int source);
        void startStream(QString url, int source);
        void stopStream();
    private:
        int m_index = -1;
        QList<int> m_cameras;
        QProcess m_v0_to_v1_and_v2;
        QProcess m_v2_to_rtmp;
    };


}


#endif // FFMPEG_H
