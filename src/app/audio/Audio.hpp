#ifndef __AUDIO_AUDIO_HPP__
#define __AUDIO_AUDIO_HPP__

#include <QDebug>
#include <QDir>
#include <QTextStream>
#include <QUrl>
#include <linphone++/linphone.hh>

#include "../setup/Configuration.hpp"

class Audio : public Configuration {
    Q_OBJECT
   private:
    std::shared_ptr<linphone::Core> _core;

   public:
    Audio(const Audio &) = delete;
    Audio &operator=(const Audio &) = delete;
    Audio(const Audio &&) = delete;
    Audio &operator=(const Audio &&) = delete;
    Audio(std::shared_ptr<linphone::Core> core);
    ~Audio() override;

    Q_INVOKABLE void playback(QString device);
    Q_INVOKABLE void capture(QString device);
    Q_INVOKABLE void play(QUrl file);

    void onSetupCompleted() override;
    void print(QTextStream &log) override;
};

#endif  // __AUDIO_AUDIO_HPP__