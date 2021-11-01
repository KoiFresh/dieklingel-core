
#include "ffmpegstream.h"
#include <QDebug>
#include <QThread>
#include <QtMultimedia/QCameraInfo>

#define LOAD_v4l2loopback "echo bigsur | sudo -S modprobe v4l2loopback devices=2"
#define UNLOAD_v4l2loopback "echo bigsur | sudo -S modprobe -r v4l2loopback"

ffmpeg::Stream::Stream()
{
    /*QProcess p;
    p.start("/bin/bash", QStringList() << "ls" << "/dev/video*" << "|" << "wc" << "-l");
    p.waitForFinished(-1);
    qDebug() << p.readAllStandardError();
    qDebug() << p.readAllStandardOutput();
    qDebug() << "cams" << m_index;
    */
    //system(UNLOAD_v4l2loopback);
    //system(LOAD_v4l2loopback);
}

ffmpeg::Stream::~Stream()
{
    //system(UNLOAD_v4l2loopback);
}

void ffmpeg::Stream::startCamera(int source, int sink)
{
    QList<int> sinks;
    sinks << sink;
    splitCamera(source, sinks);
}

void ffmpeg::Stream::splitCamera(int source, QList<int> sinks)
{
    m_cameras = sinks;
    m_v0_to_v1_and_v2.setProgram("ffmpeg");
    QStringList args;
    args << "-re" <<  "-i" << "/dev/video" + QString::number(source);
    foreach (int index, sinks)
    {
        qDebug() << "sink added" << index;
        args << "-vf" << "format=yuv420p" << "-f" << "v4l2" << "/dev/video" + QString::number(index);
    }
    m_v0_to_v1_and_v2.setArguments(args);
    //m_v0_to_v1_and_v2.setArguments(QStringList() << "-re" <<  "-i" << "/dev/video" + QString::number(source) << "-vf" << "format=yuv420p" <<  "-f" << "v4l2"  << "/dev/video" + QString::number(sink1) << "-f" << "v4l2" << "/dev/video" + QString::number(sink2));
    m_v0_to_v1_and_v2.start();
    m_v0_to_v1_and_v2.waitForStarted();
    m_index = -1;
}

QList<int> ffmpeg::Stream::getCameras()
{
    return m_cameras;
}

int ffmpeg::Stream::getNextCamera()
{
    if(m_index < m_cameras.length() - 1)
    {
        m_index++;
    }else
    {
        m_index = -1;
    }
    return (m_index >= 0) ? m_cameras[m_index] : m_index;
}

bool ffmpeg::Stream::waitForCameraReady()
{
    QThread::sleep(5);
    return  m_v0_to_v1_and_v2.waitForStarted();
}

void ffmpeg::Stream::stopCamera()
{
    m_v0_to_v1_and_v2.kill();
}

void ffmpeg::Stream::startStream(QString url, int framerate, QString resolution, int source)
{
    throw "[ffmpeg startStream] this function is not implemented yet";
}

void ffmpeg::Stream::startStream(QString url, int source)
{
    m_v2_to_rtmp.setProgram("ffmpeg");
    //m_v2_to_rtmp.setArguments(QStringList() << "-framerate" << "24" << "-s" << "1920x1080" << "-an" << "-f" << "v4l2" << "-i" << "/dev/video" + QString::number(source) << "-f" << "flv" << url);

    //m_v2_to_rtmp.setArguments(QStringList() << "-framerate" << "24" << "-s" << "320x240" << "-an" << "-f" << "v4l2" << "-i" << "/dev/video" + QString::number(source) << "-f" << "rtsp" << url);
    m_v2_to_rtmp.setArguments(QStringList() << "-f" << "v4l2" << "-i" << "/dev/video" + QString::number(source) << "-f" << "rtsp" << url);

    qDebug() << m_v2_to_rtmp.arguments();

    m_v2_to_rtmp.start();
}

void ffmpeg::Stream::stopStream()
{
    m_v2_to_rtmp.kill();
}




