#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>

#include "rate.h"
#include "ui_dialogSetUrl.h"
//#include "ui_formaTurnCameras.h"

#include "tvscreen.h"
#include "cameraviewer.h"
#include "camera.h"
#include <QThread>
#include <QPointer>
#include "playerviewer.h"
#include "lcdtimer.h"
#include <QCheckBox>

class PcScreen : public QWidget {
	Q_OBJECT



public:
    PcScreen(QWidget *parent = 0);

signals:
	void sendKey(int);

//public slots:




private slots:
    void finishedCamera(void);
    void turnCamera(bool);
    void setCam(void);
    void showSettings(void);
    void reset(void);



    void StopRecord(void);
    void StartRecord(bool);
    void PlayFile(void);
    void PlaySlowMotion(void);
    void closePlayer(void);
    void PlaySelectedFile(void);

private:
    CameraViewer* viewCam1;
    CameraViewer* viewCam2;
    Camera* camera1;
    Camera* camera2;
    QThread* threadCam1;
    QThread* threadCam2;
    QDialog * dialogSetUrl;
    QCheckBox* cbCam1;
    QCheckBox* cbCam2;
    QPointer<PlayerViewer> slowMotionPlayer;

	virtual void paintEvent(QPaintEvent *);
    virtual void closeEvent(QCloseEvent*);

    QPushButton* btnStopRecord;

    QPushButton* btnPlayLastWithSound1;
    QPushButton* btnPlayLastSlowMotion1;
    QPushButton* btnPlayLastWithSound2;
    QPushButton* btnPlayLastSlowMotion2;
    QPushButton* btnPlaySlowMotion;

	QString col_red;
	QString col_blue;

    QString cam1Url = "";
    QString cam2Url = "";

    QString currentFileCam1 = "";
    QString currentFileCam2 = "";

	QGridLayout * grid;

    Rate * rateRed;
    Rate * rateBlue;

    Ui::dlgSetUrl ui_url;

	QDesktopWidget* desk;

    TvScreen * TS;

    QThread* cam1Thread;
    QThread* cam2Thread;


    LcdTimer *mainTimer;
};
