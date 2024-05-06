#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    a.setWindowIcon(QIcon("../res/rockPaperScissorsIcon.png"));
    w.setWindowTitle("Rock Paper Scissors");
    w.show();
    return a.exec();
}
