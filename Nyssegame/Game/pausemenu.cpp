#include "pausemenu.hh"
#include "ui_pausemenu.h"
#include "startmenu.hh"

PauseMenu::PauseMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PauseMenu)
{
    ui->setupUi(this);
}

PauseMenu::~PauseMenu()
{
    delete ui;
}

void PauseMenu::on_StartBtn_clicked()
{
    PauseMenu::accept();
}

void PauseMenu::on_ExitBtn_clicked()
{
    PauseMenu::reject();
}
