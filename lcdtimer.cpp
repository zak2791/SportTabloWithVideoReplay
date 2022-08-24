#include "lcdtimer.h"

LcdTimer::LcdTimer(QWidget *parent,
                   QColor color_stopped,
                   QColor color_works,
                   QString strInitTime) : QLCDNumber(parent){

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));

    status = 0;

    intInitTime = strTimeToInt(strInitTime);
    time = intInitTime;

    palStopped = QPalette();
    palStopped.setColor(QPalette::Window, QColor("black"));
    palStopped.setColor(QPalette::WindowText, color_stopped);

    palWorks = QPalette();
    palWorks.setColor(QPalette::Window, QColor("black"));
    palWorks.setColor(QPalette::WindowText, color_works);

    setPalette(palStopped);
    setAutoFillBackground(true);

    setDigitCount(4);
    Reset();

}

void LcdTimer::StartStop(){
    if(timer->isActive()) {
        timer->stop();
        status = 2;
        setPalette(palStopped);
        sigStopped(true);
    }
    else {
        if (status != 3) {
            timer->start(1000);
            status = 1;
            setPalette(palWorks);
            sigStopped(false);
        }
    }
    emit sigTime(intTimeToStr(time), palette());
}

void LcdTimer::Reset(){
    if (status != 1) {
        time = intInitTime;
        QString sTime = intTimeToStr(intInitTime);
        display(sTime);
        emit sigTime(sTime, palette());
        sigReset();
    }
}

void LcdTimer::showTime(){
    if(--time == 0){
        status = 3;
        timer->stop();
        setPalette(palStopped);
    }
    QString sTime = intTimeToStr(time);
    display(sTime);
    emit sigTime(sTime, palette());
}

void LcdTimer::showTime(QString sTime, QPalette pal){
    setPalette(pal);
    display(sTime);
}

int LcdTimer::getStatus(){
    return status;
}

int LcdTimer::strTimeToInt(QString sTime){
    QStringList lTime = sTime.split(":");
    if(lTime.length() != 2)
        return 0;
    bool ok;
    int minutes = lTime[0].toInt(&ok);
    if(!ok)
        return 0;
    int seconds = lTime[1].toInt(&ok);
    if(!ok)
        return 0;
    return minutes * 60 + seconds;
}

QString LcdTimer::intTimeToStr(int iTime){
    int minutes = iTime / 60;
    int secondes = iTime - minutes * 60;
    QString sSeconds;
    if(secondes < 10) sSeconds = "0" + QString::number(secondes);
    else sSeconds = QString::number(secondes);
    return QString::number(minutes) + ":" + sSeconds;
}
