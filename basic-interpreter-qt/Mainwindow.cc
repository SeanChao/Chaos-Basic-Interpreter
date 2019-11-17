#include "mainwindow.h"
#include "Console.h"
#include "editor/Editor.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    console = new Console(this);
    console->setGeometry(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    console->write("Hello, world!");

    editor = new Editor;
    connect(console, &Console::newLineWritten, editor, &Editor::getUserInput);
}

MainWindow::~MainWindow() { delete console; }
