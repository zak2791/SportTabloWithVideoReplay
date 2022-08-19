#include "rate.h"
#include "lcdtimer.h"

class TvScreen : public QWidget {
	Q_OBJECT
public:
    TvScreen(QWidget *parent = 0);

    Rate * rateRed;
    Rate * rateBlue;

    LcdTimer * mainTimer;


private:
	virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);

};
