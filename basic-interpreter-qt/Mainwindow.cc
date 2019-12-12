#include "Mainwindow.h"
#include "Console.h"
#include "editor/Editor.h"
#include "interpreter/program.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    Program *program = new Program;

    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    console = new Console(this);
    console->setGeometry(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    console->write("Hello, world!\ncmd>\n");

    editor = new Editor(program);
    connect(editor, &Editor::editorOutput, console, &Console::getOutput);
    connect(console, &Console::newLineWritten, editor, &Editor::getUserInput);
}

MainWindow::~MainWindow() { delete console; }
