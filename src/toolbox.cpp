#include <QMetaProperty>
#include <QJsonDocument>
#include <QJsonObject>
#include "toolbox.h"
#include "base64.h"
#include <opencv2/opencv.hpp>

char *dieklingel::Toolbox::QString_to_Char(QString value)
{
    QByteArray val_as_ba = value.toLocal8Bit();
    return  val_as_ba.data();
}

QString dieklingel::Toolbox::s_mat_to_b64qstring(cv::Mat mat)
{
    QString result = "";
    if(!mat.empty())
    {
        std::vector<uchar> buf;
        cv::imencode(".png", mat, buf);
        uchar *enc = new uchar[buf.size()];
        for(unsigned long i = 0; i < buf.size(); i++)
        {
            enc[i] = buf[i];
        }
        result = (base64_encode(enc, buf.size())).c_str();
    }
    return  result;
}

QByteArray dieklingel::Toolbox::s_mat_to_qbytearray(cv::Mat mat) 
{
    // this is default working
    /*std::vector<uchar> buf;
    cv::imencode(".png", mat, buf);
    char *enc = new char[buf.size()];
    for(unsigned long i = 0; i < buf.size(); i++)
    {
        enc[i] = buf[i];
    }
    return QByteArray(reinterpret_cast<char*>(enc), buf.size()); */
    // this is customized to compress
    std::vector<uchar> buf;
    std::vector<int> param(2);
    param[0] = cv::IMWRITE_JPEG_QUALITY;
    //param[0] = cv::IMWRITE_PNG_COMPRESSION;
    param[1] = 30;
    cv::imencode(".jpg", mat, buf, param);
    char *enc = new char[buf.size()];
    for(unsigned long i = 0; i < buf.size(); i++)
    {
        enc[i] = buf[i];
    }
    return QByteArray(reinterpret_cast<char*>(enc), buf.size()); 
}

