#ifndef __INACTIVITY_DETECTOR_HPP__
#define __INACTIVITY_DETECTOR_HPP__

#include <QEvent>
#include <QLoggingCategory>
#include <QObject>
#include <QTimer>

class InactivityDetector : public QObject {
    Q_OBJECT
   private:
    QTimer _timer;

   public:
    InactivityDetector(const InactivityDetector &) = delete;
    InactivityDetector &operator=(const InactivityDetector &) = delete;
    InactivityDetector(const InactivityDetector &&) = delete;
    InactivityDetector &operator=(const InactivityDetector &&) = delete;
    InactivityDetector();
    ~InactivityDetector() override;

   signals:
    void inactivity();

   protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif  // __INACTIVITY_DETECTOR_HPP__