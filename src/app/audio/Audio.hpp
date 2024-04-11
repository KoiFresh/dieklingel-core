#ifndef __AUDIO_AUDIO_HPP__
#define __AUDIO_AUDIO_HPP__

#include <QTextStream>
#include <linphone++/linphone.hh>

#include "../setup/Configuration.hpp"

class Audio : public Configuration {
    Q_OBJECT
   private:
    std::shared_ptr<linphone::Core> _core;

   public:
    Audio(std::shared_ptr<linphone::Core> core);
    ~Audio();

    Q_INVOKABLE void playback(QString device);
    Q_INVOKABLE void capture(QString device);

    void onSetupCompleted() override;
    void print(QTextStream &log) override;
};

#endif  // __AUDIO_AUDIO_HPP__