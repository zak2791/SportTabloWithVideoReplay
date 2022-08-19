#ifndef CAMERAVIEWER_H
#define CAMERAVIEWER_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QWidget>

class CameraViewer : public QGraphicsView
{
    Q_OBJECT
public:
    CameraViewer(QWidget* parent=nullptr);

private:
    //QQueue<QImage> que;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *pi;
    QWidget *p;

public slots:
    void draw_image(QImage);

};

#endif // CAMERAVIEWER_H
