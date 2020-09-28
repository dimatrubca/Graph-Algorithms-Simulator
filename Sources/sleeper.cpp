#include "sleeper.h"
#include <QtCore>
#include <QDebug>

Sleeper::Sleeper(QObject *parent) : QObject(parent), stopped(0)
{}

bool Sleeper::sleep(int msec){
    if(timer.isActive() || stopped) return !(stopped=true);

    timer.start(msec, this);
    while(timer.isActive())
        QCoreApplication::processEvents();

    return !stopped;
}

void Sleeper::stop(){
    stopped = true;
    timer.stop();
}

void Sleeper::timerEvent(QTimerEvent *ev){
    if(ev->timerId() == timer.timerId())
        timer.stop();
}
