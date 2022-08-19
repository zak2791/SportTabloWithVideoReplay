#include <QObject>
#include "pcscreen.h"

int main(int argc, char** argv){
    QApplication app(argc, argv);

    PcScreen * pwgt = new PcScreen();

	return app.exec();
}

