#include <QApplication>
#include "Mainwindow.h"
#include "editor/Editor.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
//    Editor ed;
    MainWindow w;
    w.show();

    return a.exec();
}
