#include "widget.h"
#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);
    QSurfaceFormat::setDefaultFormat(format);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    MainWindow mw;
    mw.show();

    return a.exec();
}
