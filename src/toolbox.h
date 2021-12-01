#ifndef TOOLBOX_H
#define TOOLBOX_H

/**
 * @author Kai Mayer
 * @version 1.0
 * Toolbox to convert some datatypes
 */

#include <QString>
#include <QObject>
#include <opencv2/opencv.hpp>

namespace dieklingel
{
    class Toolbox
    {
    public:
        static char* QString_to_Char(QString value);
        static QString s_mat_to_b64qstring(cv::Mat mat);
        static QByteArray s_mat_to_qbytearray(cv::Mat mat);

    private:
        Toolbox() {};
    };
}


#endif // TOOLBOX_H
