#ifndef __SETUP_CONFIGURATION_HPP__
#define __SETUP_CONFIGURATION_HPP__

#include <QObject>

class Configuration : public QObject {
   public:
    virtual void onSetupCompleted() = 0;
    virtual void print(QTextStream& log) = 0;
};

#endif  // __SETUP_CONFIGURATION_HPP__
