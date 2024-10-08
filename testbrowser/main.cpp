// main.cpp
#include <QApplication>
#include <QtWebEngineQuick/QtWebEngineQuick>
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Ініціалізуємо QtWebEngine для Qt6
    QtWebEngineQuick::initialize();

    MainWindow window;
    window.show();

    return app.exec();
}
