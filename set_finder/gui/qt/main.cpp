#include <QApplication>

#include <QCamera>
#include <QDebug>
#include <QMediaCaptureSession>
#include <QQmlApplicationEngine>
#include <QScreen>
#include <QVideoWidget>

#include "myitem.h"

int main(int argc, char* argv[])
{
    //    QApplication app(argc, argv);
    QGuiApplication app(argc, argv);

    //    MyItem myItem;
    //    myItem.doNothing();

    //    QScreen* screen = QApplication::primaryScreen();

    //    const auto& width = screen->availableGeometry().width();
    //    const auto& height = screen->availableGeometry().height();

    //    const auto& widthSize = screen->availableSize().width();
    //    const auto& heightSize = screen->availableSize().height();

    //    QMediaCaptureSession captureSession;

    //    auto* camera = new QCamera;

    //    captureSession.setCamera(camera);
    //    auto cameraDevice = camera->cameraDevice();
    //    auto formats = cameraDevice.videoFormats();
    //    camera->setCameraFormat(formats.at(20));

    //    auto* viewfinder = new QVideoWidget;
    //    viewfinder->setAspectRatioMode(Qt::KeepAspectRatio);
    //    captureSession.setVideoOutput(viewfinder);
    //    viewfinder->show();

    //    camera->start();

    //    int counter = 0;
    //    qDebug() << "!!!!!!!!!!!!! availableGeometry: " << width << height;
    //    qDebug() << "!!!!!!!!!!!!! availableSize: " << widthSize << heightSize;
    //    qDebug() << "!!!!!!!!!!!!! devicePixelRatio: " << screen->devicePixelRatio();

    //    auto photoResolutions = cameraDevice.photoResolutions();

    //    for(const auto& format : photoResolutions)
    //    {
    //        qDebug() << "!!!!!!!!!!!!!: " << counter++ << format;
    //    }

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/gui/main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if(!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
