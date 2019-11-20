#include "Console.h"
#include <QDebug>
#include <QKeyEvent>
#include <QTextCursor>
#include <QTextLine>

Console::Console(QWidget *parent) : QTextEdit(parent) {}

void Console::clear() { this->clear(); }

void Console::write(QString msg) {
    this->append(msg);
    // qDebug() << "console <- " << msg << '\n';
}

void Console::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Backspace) return;
    if (event->key() == Qt::Key_Delete) return;
    if (this->textCursor().hasSelection()) return;
    if (event->key() == Qt::Key_Return) {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        QString lastLine = cursor.selectedText();
        newLineWritten(lastLine);
    }
    QTextEdit::keyPressEvent(event);
}

/**
 * SLOT: get the output stream from the editor to print
 * @param QString out: the output string
 */
void Console::getOutput(QString out) {
    qDebug() << "GET FR ED: " << out;
    write(out);
}
