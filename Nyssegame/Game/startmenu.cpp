#include "startmenu.hh"
#include "ui_startmenu.h"

StartMenu::StartMenu(QString* playername, QList <std::pair<int, std::string>>* scores,  QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartMenu)
{
    ui->setupUi(this);
    playername_ = playername;
    QImage startmenu = QImage("../../codecookingkitchen/symbols/startmenu.png");
    scene = new QGraphicsScene(0,0,1200,720);
    scene->setBackgroundBrush(startmenu);
    ui->graphicsView->setGeometry(0,0,1200,720);
    ui->graphicsView->setScene(scene);

    ui->ExitBtn->setGeometry(0, 0, 50, 25);
    ui->StartBtn->setGeometry(1100, 0, 100, 25);
    ui->StartBtn->setDisabled(1);
    QString s = "";

    if(scores->size() <= 10){
        for (int i = 0; i < scores->size(); i++){
            s = s + QString::number(i+ 1) + ". " + QString::fromStdString(scores->at(i).second) + "\n";
        }
    }
    else{
        for (int i = 0; i < 10; i++){
            s = s + QString::number(i+ 1) + ". " + QString::fromStdString(scores->at(i).second) + "\n";
        }
    }

    ui->scores->setText(s);
}

StartMenu::~StartMenu()
{
    delete ui;
    delete scene;
}

void StartMenu::on_ExitBtn_clicked()
{
    StartMenu::reject();
}

void StartMenu::on_StartBtn_clicked()
{
    emit Difficulty(difficulty);
    StartMenu::accept();
    ui->StartBtn->setEnabled(0);

}

void StartMenu::on_easyBtn_clicked()
{
    ui->StartBtn->setEnabled(1);
    difficulty = EASY;
}

void StartMenu::on_mediumBtn_clicked()
{
    ui->StartBtn->setEnabled(1);
    difficulty = MEDIUM;
}

void StartMenu::on_hardBtn_clicked()
{
    ui->StartBtn->setEnabled(1);
    difficulty = HARD;
}


void StartMenu::on_lineEdit_editingFinished()
{
    QString line = ui->lineEdit->text();
    *playername_ = line;
}
