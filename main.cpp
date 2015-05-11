#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml/qqml.h>
#include "opencvcamera.h"
#include "opencvshowframe.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<OpenCVcamera>("OpenCV", 1, 0, "OpenCVcamera");
    qmlRegisterType<OpenCVshowFrame>("OpenCV", 1, 0, "OpenCVshowFrame");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
