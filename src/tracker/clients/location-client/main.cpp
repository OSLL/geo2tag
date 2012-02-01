#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    const QString mainQmlApp = QLatin1String("qrc:///mapviewer.qml");
    QDeclarativeView view;
    view.setSource(QUrl(mainQmlApp));
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
    // Qt.quit() called in embedded .qml by default only emits
    // quit() signal, so do this (optionally use Qt.exit()).
    QObject::connect(view.engine(), SIGNAL(quit()), qApp, SLOT(quit()));
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_6)
    view.showFullScreen();
#else
    view.setGeometry(QRect(100, 100, 360, 640));
    view.show();
#endif
    return application.exec();
}

