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

QString dieklingel::Toolbox::Mat_to_b64QString(cv::Mat mat)
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


