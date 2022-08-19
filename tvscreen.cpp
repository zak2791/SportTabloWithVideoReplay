#include <QPainter>
#include <QGridLayout>
#include "tvscreen.h"

TvScreen::TvScreen(QWidget * parent) : QWidget(parent) {
    rateRed = new Rate(this);
    rateBlue = new Rate(this);



    mainTimer = new LcdTimer(this);

    QGridLayout * grid = new QGridLayout(this);

    grid->addWidget(rateBlue, 6, 0, 18, 24);
    grid->addWidget(rateRed, 6, 44, 18, 24);

    grid->addWidget(mainTimer,  24,  24, 13, 20);



}

void TvScreen::resizeEvent(QResizeEvent *){

}

void TvScreen::paintEvent(QPaintEvent * ) {
	QPainter pn;
    pn.begin(this);
    pn.setBrush(Qt::blue);
    pn.drawRect(0, 0, width() / 2, height());
    pn.setBrush(Qt::red);
    pn.drawRect(width() / 2, 0, width() / 2, height());
	pn.end();
}
