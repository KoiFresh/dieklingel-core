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
        static QString Mat_to_b64QString(cv::Mat mat);

    private:
        Toolbox() {};
    };
}


#endif // TOOLBOX_H
