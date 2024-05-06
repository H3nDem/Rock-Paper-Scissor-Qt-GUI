#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QRandomGenerator"
#include "QTimer"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "qdebug.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Init variables */
    res = {
        QPixmap("../res/default.png"),
        QPixmap("../res/rock.png"),
        QPixmap("../res/paper.png"),
        QPixmap("../res/scissors.png"),
    };
    playerMove = 0;
    botMove = 0;

    /* Load data for json file */
    loadData();
    roundsPlayed = wins + loses + draws;

    /* Init images */
    ui->playerMoveImage->setPixmap(res.at(0));
    ui->botMoveImage->setPixmap(res.at(0));
    ui->playerMoveImage->setScaledContents(true);
    ui->botMoveImage->setScaledContents(true);

    /* Init buttons */
    ui->rockButton->setCheckable(true);
    ui->paperButton->setCheckable(true);
    ui->scissorsButton->setCheckable(true);

    /* Init labels (those for text, the ones for images are already initialized) */
    ui->winsScore->setText("Wins : " + QString::number(wins));
    ui->losesScore->setText("Loses : " + QString::number(loses));
    ui->drawsScore->setText("Draws : " + QString::number(draws));
    ui->roundsPlayed->setText("Rounds played : " + QString::number(roundsPlayed));

    /* Connect buttons */
    connect(ui->rockButton, SIGNAL(released()), this, SLOT(play()));
    connect(ui->paperButton, SIGNAL(released()), this, SLOT(play()));
    connect(ui->scissorsButton, SIGNAL(released()), this, SLOT(play()));
}

MainWindow::~MainWindow()
{
    delete ui;
    saveData();
}

void MainWindow::play()
{
    setPlayerMove();
    setBotMove();
    checkMoves();
    ui->rockButton->setEnabled(false); // Deactivating buttons to prevent actions while waiting for the delayed function
    ui->paperButton->setEnabled(false);
    ui->scissorsButton->setEnabled(false);
    QTimer::singleShot(1500, this, &MainWindow::clearMoves);
}


void MainWindow::setPlayerMove() {
    QPushButton * button = (QPushButton *) sender();
    button->setChecked(true);

    if (ui->rockButton->isChecked()) {
        playerMove = 1;
        rockPlayed++;
    } else if (ui->paperButton->isChecked()) {
        playerMove = 2;
        paperPlayed++;
    } else if (ui->scissorsButton->isChecked()) {
        playerMove = 3;
        scissorsPlayed++;
    } else {
        playerMove = 0;
    }

    button->setChecked(false);
    ui->playerMoveImage->setPixmap(res.at(playerMove));
}

void MainWindow::setBotMove() {
    int randomMove = QRandomGenerator::global()->generate() % 3 + 1; // (value unint on 32 bits) % 3 + 1 to get between 1 and 3
    botMove = randomMove;
    ui->botMoveImage->setPixmap(res.at(botMove));
}

void MainWindow::checkMoves() {
    if (playerMove == botMove) {                    // draw
        draws++;
        ui->drawsScore->setText("Draws : " + QString::number(draws));
        ui->roundResultText->setText("Draw !");
    } else if (playerMove == 1 && botMove == 2) {   // rock - papaer
        loses++;
        ui->losesScore->setText("Loses : " + QString::number(loses));
        ui->roundResultText->setText("You lose !");
    } else if (playerMove == 1 && botMove == 3) {   // rock - scissors
        wins++;
        ui->winsScore->setText("Wins : " + QString::number(wins));
        ui->roundResultText->setText("You win !");
    } else if (playerMove == 2 && botMove == 3) {   // papaer - scissors
        loses++;
        ui->losesScore->setText("Loses : " + QString::number(loses));
        ui->roundResultText->setText("You lose !");
    } else if (playerMove == 2 && botMove == 1) {   // paper - rock
        wins++;
        ui->winsScore->setText("Wins : " + QString::number(wins));
        ui->roundResultText->setText("You win !");
    } else if (playerMove == 3 && botMove == 1) {   // scissors - rock
        loses++;
        ui->losesScore->setText("Loses : " + QString::number(loses));
        ui->roundResultText->setText("You lose !");
    } else if (playerMove == 3 && botMove == 2) {   // scissors - paper
        wins++;
        ui->winsScore->setText("Wins : " + QString::number(wins));
        ui->roundResultText->setText("You win !");
    } else {
        ui->roundResultText->setText("Error : Impossible matchup");
    }
    roundsPlayed++;
    ui->roundsPlayed->setText("Rounds played : " + QString::number(roundsPlayed));
}

void MainWindow::clearMoves() {
    playerMove = 0;
    botMove = 0;
    ui->playerMoveImage->setPixmap(res.at(0));
    ui->botMoveImage->setPixmap(res.at(0));
    ui->roundResultText->setText("");
    ui->rockButton->setEnabled(true);
    ui->paperButton->setEnabled(true);
    ui->scissorsButton->setEnabled(true);
}

/** Data management **/

void MainWindow::saveData()
{
    /* We create a dictionnary that'll contain all our data we want to save */
    QJsonObject dictionary;
    dictionary["wins"] = wins;
    dictionary["loses"] = loses;
    dictionary["draws"] = draws;
    dictionary["rockPlayed"] = rockPlayed;
    dictionary["paperPlayed"] = paperPlayed;
    dictionary["scissorsPlayed"] = scissorsPlayed;

    /* We convert the dictionnary into string in order to be wriiten in a file, we keep indentation for better readablily */
    QJsonDocument jsonDoc(dictionary);
    QString dataString = jsonDoc.toJson(QJsonDocument::Indented);

    /* We open this file in write only mode, and write all the data string in it */
    QFile file("../data.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << dataString;
        file.flush();
        file.close();
        qDebug() << "File written successfully.";
    } else {
        qDebug() << "Failed to open file.";
    }
}

void MainWindow::loadData()
{
    /* Gets the stored data into a string */
    QString dataString;
    QFile file("../data.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        dataString = file.readAll();
        file.close();
        qDebug() << "Data loaded successfully.";
    } else {
        qDebug() << "Failed to open file.";
        return;
    }

    /* Convert data string to a dictionnary and apply load onto variables to be used throughout the code */
    QJsonDocument jsonDoc = QJsonDocument::fromJson(dataString.toUtf8());
    QJsonObject dictionary = jsonDoc.object();
    wins = dictionary["wins"].toInt();
    loses = dictionary["loses"].toInt();
    draws = dictionary["draws"].toInt();
    rockPlayed = dictionary["rockPlayed"].toInt();
    paperPlayed = dictionary["paperPlayed"].toInt();
    scissorsPlayed = dictionary["scissorsPlayed"].toInt();
}
