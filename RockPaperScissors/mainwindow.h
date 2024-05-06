#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<QPixmap> res;     // image ressources
    int playerMove;
    int botMove;
    QJsonObject dictionary; // dictionnary used to load and save data in variables below
    int wins;
    int loses;
    int draws;
    int rockPlayed;         // how many rocks did you play, same for each move
    int paperPlayed;
    int scissorsPlayed;
    int roundsPlayed;       // no need to save since we can get it by adding wins, loses and draws
    // player w/l = bot l/w

private slots:
    void play();
    void setPlayerMove();
    void setBotMove();
    void checkMoves();
    void clearMoves();
    void saveData();
    void loadData();
};
#endif // MAINWINDOW_H
