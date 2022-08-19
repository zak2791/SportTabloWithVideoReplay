#ifndef LCDTIMER_H
#define LCDTIMER_H

#include <QLCDNumber>
#include <QWidget>
#include <QPalette>
#include <QTimer>

class LcdTimer : public QLCDNumber
{
    Q_OBJECT
public:
    LcdTimer(QWidget *parent = 0,
             QColor color_stopped = QColor(0, 150, 0),
             QColor color_works = QColor(0, 255, 0),
             QString initTime = "3:00");


    void Reset();
    int getStatus(void);

private:
    int status;     //0 - reset state
                    //1 - works
                    //2 - stopped
                    //3 - final state
    QString strInitTime;
    int time;
    int intInitTime;
    QPalette palStopped;
    QPalette palWorks;
    QTimer *timer;

    int strTimeToInt(QString);
    QString intTimeToStr(int);

public slots:
    void StartStop();
    void showTime(QString, QPalette);

private slots:
    void showTime(void);

signals:
    void sigTime(QString, QPalette);
    void sigStopped(bool);
    void sigReset(void);

};

#endif // LCDTIMER_H
