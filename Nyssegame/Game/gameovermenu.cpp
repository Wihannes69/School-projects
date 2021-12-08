#include "gameovermenu.h"
#include "ui_gameovermenu.h"

GameOverMenu::GameOverMenu(Statistics* gamestats, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameOverMenu)
{
    ui->setupUi(this);

    QString passengers = QString::number(gamestats->givePassengerKillCount());
    QString nysses = QString::number(gamestats->giveNysseKillCount());
    QString text = "Passengers killed: " + passengers
            + "\nNysses killed: " + nysses;
    ui->statslabel->setText(text);
}

GameOverMenu::~GameOverMenu()
{
    delete ui;
}

void GameOverMenu::on_exitButton_clicked()
{
    // exit the game
    GameOverMenu::reject();
}

void GameOverMenu::on_restartButton_clicked()
{
    GameOverMenu::accept();
}
