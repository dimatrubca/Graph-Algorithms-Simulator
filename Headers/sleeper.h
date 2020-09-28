#ifndef SLEEPER_H
#define SLEEPER_H

#include <QObject>
#include <QBasicTimer>

class Sleeper : public QObject
{
    Q_OBJECT
public:
    explicit Sleeper(QObject *parent = nullptr);

    inline bool isStopped() { return stopped; }
public slots:
    bool sleep(int msec);
    void stop();


private:
    void timerEvent(QTimerEvent *ev);
    QBasicTimer timer;
    bool stopped;
};

#endif // SLEEPER_H
