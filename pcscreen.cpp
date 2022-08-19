#include <QPainter>
#include <QGridLayout>
#include <QPushButton>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QPalette>
#include <QProcess>
#include <QTime>
#include <QDir>

#include "pcscreen.h"


#include <math.h>

PcScreen::PcScreen(QWidget * parent) : QWidget(parent){
	col_red = "white";
	col_blue = "white";

    rateRed = new Rate(this);

    rateBlue = new Rate(this);

    QPushButton * btnReset = new QPushButton(u8"СБРОС", this);
    btnReset->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    btnReset->setStyleSheet("color: red; font: bold " + QString::number(round(btnReset->height() / 2)) + "px;");
    btnReset->setFocusPolicy(Qt::NoFocus);
    connect(btnReset, SIGNAL(clicked()), this, SLOT(reset()));

	QPushButton * btnTime = new QPushButton(u8"ВРЕМЯ", this);
	btnTime->setObjectName("btnTime");
	btnTime->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    btnTime->setStyleSheet("color: green; font: bold " + QString::number(round(btnTime->height() / 2)) + "px;");
	btnTime->setFocusPolicy(Qt::NoFocus);

    dialogSetUrl = new QDialog(this);
    ui_url.setupUi(dialogSetUrl);

    QPushButton * btnSettings = new QPushButton(u8"НАСТРОЙКИ", this);
    btnSettings->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    btnSettings->setStyleSheet("font: bold " + QString::number(round(btnSettings->height() / 2)) + "px;");
    btnSettings->setFocusPolicy(Qt::NoFocus);
    connect(btnSettings, SIGNAL(clicked(bool)), this, SLOT(showSettings()));



    QPushButton * btnExit = new QPushButton("ВЫХОД", this);
    btnExit->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    btnExit->setStyleSheet("font: bold " + QString::number(round(btnExit->height() / 2)) + "px;");
    btnExit->setFocusPolicy(Qt::NoFocus);
    connect(btnExit, SIGNAL(clicked(bool)), this, SLOT(close()));

    viewCam1 = new CameraViewer;
    viewCam1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    viewCam2 = new CameraViewer;
    viewCam2->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    btnPlayLastWithSound1 = new QPushButton("Просмотр\nсо звуком");
    btnPlayLastWithSound1->setObjectName("btnPlayLastWithSound1");
    btnPlayLastWithSound1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    connect(btnPlayLastWithSound1, SIGNAL(clicked()), SLOT(PlayFile()));

    btnPlayLastWithSound2 = new QPushButton("Просмотр\nсо звуком");
    btnPlayLastWithSound2->setObjectName("btnPlayLastWithSound2");
    btnPlayLastWithSound2->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    connect(btnPlayLastWithSound2, SIGNAL(clicked()), SLOT(PlayFile()));

    btnPlayLastSlowMotion1 = new QPushButton("Просмотр\nзамедленный");
    btnPlayLastSlowMotion1->setObjectName("btnPlayLastSlowMotion1");
    btnPlayLastSlowMotion1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    connect(btnPlayLastSlowMotion1, SIGNAL(clicked()), SLOT(PlaySlowMotion()));

    btnPlayLastSlowMotion2 = new QPushButton("Просмотр\nзамедленный");
    btnPlayLastSlowMotion2->setObjectName("btnPlayLastSlowMotion2");
    btnPlayLastSlowMotion2->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    connect(btnPlayLastSlowMotion2, SIGNAL(clicked()), SLOT(PlaySlowMotion()));

    btnPlaySlowMotion = new QPushButton("Просмотр с выбором файла");
    btnPlaySlowMotion->setObjectName("btnPlaySlowMotion");
    btnPlaySlowMotion->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    connect(btnPlaySlowMotion, SIGNAL(clicked()), SLOT(PlaySelectedFile()));

    mainTimer = new LcdTimer(this);
    connect(btnTime, SIGNAL(clicked()), mainTimer, SLOT(StartStop()));

    btnStopRecord = new QPushButton("Стоп запись");
    btnStopRecord->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    connect(btnStopRecord, SIGNAL(clicked()), SLOT(StopRecord()));
    connect(mainTimer, SIGNAL(sigStopped(bool)), btnStopRecord, SLOT(setEnabled(bool)));

	grid = new QGridLayout(this);

    cbCam1 = new QCheckBox("Включить камеру 1");
    cbCam1->setObjectName("cbCam1");
    cbCam1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    connect(cbCam1, SIGNAL(toggled(bool)), this, SLOT(turnCamera(bool)));

    cbCam2 = new QCheckBox("Включить камеру 2");
    cbCam2->setObjectName("cbCam2");
    cbCam2->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    connect(cbCam2, SIGNAL(toggled(bool)), this, SLOT(turnCamera(bool)));

    QLabel* lbl = new QLabel("Последний записанный файл");
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setStyleSheet("color: white; border-style: solid;"
                                     "border-width: 2px;"
                                     "border-color: white;"
                                     "border-top-style: none;"
                                     "border-bottom-left-radius: 5px;"
                                     "border-bottom-right-radius: 5px;"
                                     "font-size: 12pt");

    //grid 68х42
	grid->setSpacing(6);
	grid->setMargin(6);
    grid->addWidget(rateRed,                6,  0,  18, 24);
    grid->addWidget(rateBlue,               6,  44, 18, 24);
    grid->addWidget(btnExit,                27, 0, 2,  10);
    grid->addWidget(btnSettings,            24, 0, 2,  10);
    grid->addWidget(btnTime,                24, 24, 2,  10);
    grid->addWidget(btnReset,               24, 34, 2,  10);

    grid->addWidget(mainTimer,              26, 24, 11, 20);
    grid->addWidget(viewCam1,               13, 24,  6, 10);
    grid->addWidget(btnPlayLastWithSound1,  19, 24,  2,  5);
    grid->addWidget(btnPlayLastSlowMotion1, 19, 29,  2,  5);
    grid->addWidget(btnPlaySlowMotion,      22, 24,  2, 10);
    grid->addWidget(cbCam1,                 18, 25,  1,  8);
    grid->addWidget(viewCam2,               13, 34,  6, 10);
    grid->addWidget(btnPlayLastWithSound2,  19, 34,  2,  5);
    grid->addWidget(btnPlayLastSlowMotion2, 19, 39,  2,  5);
    grid->addWidget(btnStopRecord,          22, 34,  2, 10);
    grid->addWidget(cbCam2,                 18, 35,  1,  8);

    grid->addWidget(lbl,                    21, 24,  1, 20);

    btnTime->setStyleSheet("color: green; font: bold " + QString::number(round(btnTime->height() / 2)) + "px;");

    //desk = QApplication::desktop();

    //if (desk->numScreens() == 2){
    if(QGuiApplication::screens().count() == 2){
        showFullScreen();
    }else{
        show();
    }

    //if (desk->numScreens() == 1) {
    if(QGuiApplication::screens().count() == 1){
        QMessageBox::information(this, u8"ВНИМАНИЕ!",
        u8"Подключите к компьютеру дисплей в режиме \"Расширенный рабочий стол!\"",
		QMessageBox::Ok);
	}
    //else {
        TS = new TvScreen();

    //if (desk->numScreens() == 1)
    if(QGuiApplication::screens().count() == 1)
        TS->setGeometry(0, 0, QApplication::desktop()->availableGeometry(this).width() / 2, QApplication::desktop()->availableGeometry(this).height() / 2);
    else{
        TS->setGeometry(width(), 0, 100, height());
        TS->setGeometry(QApplication::desktop()->availableGeometry(this).right(),
                        0, QApplication::desktop()->availableGeometry(TS).width(),
                        QApplication::desktop()->availableGeometry(TS).height());
    }

        TS->show();

        connect(rateRed,	SIGNAL(sigRate(int)),		TS->rateRed,	SLOT(setRate(int)));
        connect(rateBlue,	SIGNAL(sigRate(int)),		TS->rateBlue,	SLOT(setRate(int)));

        connect(mainTimer, SIGNAL(sigTime(QString, QPalette)), TS->mainTimer, SLOT(showTime(QString, QPalette)));


    //}



    //camLeftThread->start();
    ui_url.leUrl1->setObjectName("leUrl1");
    ui_url.leUrl2->setObjectName("leUrl2");
    connect(ui_url.leUrl1, SIGNAL(editingFinished()), this, SLOT(setCam()));
    connect(ui_url.leUrl2, SIGNAL(editingFinished()), this, SLOT(setCam()));

    QFile cam1("cam1.txt");
    if(!cam1.exists()){
        cam1.open(QIODevice::WriteOnly);
        cam1.close();
    }
    else{
        cam1.open(QIODevice::ReadOnly);
        cam1Url = cam1.readAll();
        cam1.close();
    }

    QFile cam2("cam2.txt");
    if(!cam2.exists()){
        cam2.open(QIODevice::WriteOnly);
        cam2.close();
    }
    else{
        cam2.open(QIODevice::ReadOnly);
        cam2Url = cam2.readAll();
        cam2.close();
    }

    camera1 = new Camera;
    camera1->setObjectName("camera1");
    threadCam1 = new QThread;
    camera1->moveToThread(threadCam1);
    connect(threadCam1, SIGNAL(started()), camera1, SLOT(TurnOnCamera()));
    connect(camera1, SIGNAL(sigImage(QImage)), viewCam1, SLOT(draw_image(QImage)));
    connect(camera1, SIGNAL(finished()), this, SLOT(finishedCamera()));

    camera2 = new Camera;
    camera2->setObjectName("camera2");
    threadCam2 = new QThread;
    camera2->moveToThread(threadCam2);
    connect(threadCam2, SIGNAL(started()), camera2, SLOT(TurnOnCamera()));
    connect(camera2, SIGNAL(sigImage(QImage)), viewCam2, SLOT(draw_image(QImage)));
    connect(camera2, SIGNAL(finished()), this, SLOT(finishedCamera()));

    connect(mainTimer, SIGNAL(sigStopped(bool)), this, SLOT(StartRecord(bool)));
    connect(mainTimer, SIGNAL(sigReset()), this, SLOT(StopRecord()));

    //QList<QWidget*> widgetList = this->findChildren<QWidget*>();
    //foreach( QWidget* widgetPtr, widgetList )
        //widgetPtr->setFocusPolicy(Qt::NoFocus);
    //this->setFocus();
    QDir dir("video");
    if(!dir.exists()){
        QDir dir;
        dir.mkdir("video");
    }
}

void PcScreen::StartRecord(bool b){
    if(b) return;
    QString currentTime = QTime::currentTime().toString().replace(":", "_");
    if(threadCam1->isRunning()){
        currentFileCam1 = "video/cam1_" + currentTime + ".mp4";
        camera1->StartRecord(currentFileCam1);
        btnPlayLastWithSound1->setEnabled(false);
        btnPlayLastSlowMotion1->setEnabled(false);
        btnPlayLastWithSound2->setEnabled(false);
        btnPlayLastSlowMotion2->setEnabled(false);
        btnPlaySlowMotion->setEnabled(false);
    }
    if(threadCam2->isRunning()){
        currentFileCam2 = "video/cam2_" + currentTime + ".mp4";
        camera2->StartRecord(currentFileCam2);
        btnPlayLastWithSound1->setEnabled(false);
        btnPlayLastSlowMotion1->setEnabled(false);
        btnPlayLastWithSound2->setEnabled(false);
        btnPlayLastSlowMotion2->setEnabled(false);
        btnPlaySlowMotion->setEnabled(false);
    }
}

void PcScreen::StopRecord(){
    camera1->StopRecord();
    camera2->StopRecord();
    camera2->StartRecord(currentFileCam2);
    btnPlayLastWithSound1->setEnabled(true);
    btnPlayLastSlowMotion1->setEnabled(true);
    btnPlayLastWithSound2->setEnabled(true);
    btnPlayLastSlowMotion2->setEnabled(true);
    btnPlaySlowMotion->setEnabled(true);
}

void PcScreen::PlayFile(){
    QProcess proc;
    QStringList l;
    l.append(QString("-fs"));
    if(sender()->objectName() == "btnPlayLastWithSound1")
        l.append(currentFileCam1);
    else
        l.append(currentFileCam2);
    proc.start("ffplay", l);
    proc.waitForFinished(-1);
}

void PcScreen::PlaySlowMotion(){
    if(!slowMotionPlayer){
        if(sender()->objectName() == "btnPlayLastWithSound1"){
            slowMotionPlayer = new PlayerViewer(currentFileCam1);
            connect(slowMotionPlayer, SIGNAL(sigClose()), this, SLOT(closePlayer()));
            btnPlayLastWithSound1->setEnabled(false);
            btnPlayLastSlowMotion1->setEnabled(false);
            btnPlayLastWithSound2->setEnabled(false);
            btnPlayLastSlowMotion2->setEnabled(false);
            btnPlaySlowMotion->setEnabled(false);
        }else{
            slowMotionPlayer = new PlayerViewer(currentFileCam2);
            connect(slowMotionPlayer, SIGNAL(sigClose()), this, SLOT(closePlayer()));
            btnPlayLastWithSound1->setEnabled(false);
            btnPlayLastSlowMotion1->setEnabled(false);
            btnPlayLastWithSound2->setEnabled(false);
            btnPlayLastSlowMotion2->setEnabled(false);
            btnPlaySlowMotion->setEnabled(false);
        }
    }
}

void PcScreen::PlaySelectedFile(){
    QString file = QFileDialog::getOpenFileName();
    if(file == "" || !file.endsWith(".mp4"))
        return;
    slowMotionPlayer = new PlayerViewer(file);
    connect(slowMotionPlayer, SIGNAL(sigClose()), this, SLOT(closePlayer()));
}

void PcScreen::closePlayer(){
    slowMotionPlayer->deleteLater();
    btnPlayLastWithSound1->setEnabled(true);
    btnPlayLastSlowMotion1->setEnabled(true);
    btnPlayLastWithSound2->setEnabled(true);
    btnPlayLastSlowMotion2->setEnabled(true);
    btnPlaySlowMotion->setEnabled(true);
}

void PcScreen::showSettings(){
    QFile f1("cam1.txt");
    f1.open(QIODevice::ReadOnly);
    ui_url.leUrl1->setText(f1.readLine());
    f1.close();
    QFile f2("cam2.txt");
    f2.open(QIODevice::ReadOnly);
    ui_url.leUrl2->setText(f2.readLine());
    f2.close();
    dialogSetUrl->show();
}

void PcScreen::finishedCamera(){
    if(sender()->objectName() == "camera1"){
        threadCam1->quit();
        threadCam1->wait();
        if(cbCam1->isChecked())
            cbCam1->toggle();
    }else{
        threadCam2->quit();
        threadCam2->wait();
        if(cbCam2->isChecked())
            cbCam2->toggle();
    }
}

void PcScreen::setCam(){
    QFile f;
    QTextStream out(&f);
    if(sender()->objectName() == "leUrl1"){
        f.setFileName("cam1.txt");
        f.open(QIODevice::WriteOnly);
        out << ui_url.leUrl1->text();
        cam1Url = ui_url.leUrl1->text();
    }else{
        f.setFileName("cam2.txt");
        f.open(QIODevice::WriteOnly);
        out << ui_url.leUrl2->text();
        cam2Url = ui_url.leUrl2->text();
    }
    f.close();
}

void PcScreen::turnCamera(bool state){
    if(state){
        if(sender()->objectName() == "cbCam1"){
            camera1->setUrl(cam1Url);
            threadCam1->start();
        }else{
            camera2->setUrl(cam2Url);
            threadCam2->start();
        }
    }
    else{
        if(sender()->objectName() == "cbCam1"){
            camera1->TurnOffCamera();
        }else{
            camera2->TurnOffCamera();
        }
    }
}

void PcScreen::paintEvent(QPaintEvent * ) {
	QPainter pn;
	pn.begin(this);
    pn.setBrush(Qt::red);
    pn.drawRect(0, 0, width() / 2, height());
    pn.setBrush(Qt::blue);
    pn.drawRect(width() / 2, 0, width() / 2, height());
	pn.end();
}

void PcScreen::closeEvent(QCloseEvent*){
    if(cbCam1->isChecked()){
        cbCam1->toggle();
        threadCam1->quit();
        threadCam1->wait();
    }
    if(cbCam2->isChecked()){
        cbCam2->toggle();
        threadCam2->quit();
        threadCam2->wait();
    }
    qApp->quit();
}

void PcScreen::reset(){
    if(mainTimer->getStatus() != 1){
        mainTimer->Reset();
        rateRed->sbros();
        rateBlue->sbros();
    }
}
