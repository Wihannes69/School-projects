/* Nysse: StartMenu
 * ----------
 * Program author
 * Name: Johannes Kalliokoski
 * Name: Mika Pajala
 *
 * Game's start menu -dialog; choose settings and start the game, or exit.
 * */
#ifndef DIALOG_HH
#define DIALOG_HH

#include <QDialog>
#include <QGraphicsScene>
#include "parameters.h"

namespace Ui {
class StartMenu;
}

class StartMenu : public QDialog
{
    Q_OBJECT

public:
    explicit StartMenu(QString*, QList <std::pair<int, std::string>>*,  QWidget *parent = nullptr);
    ~StartMenu();

signals:
    void Difficulty(int);


private slots:
    void on_ExitBtn_clicked();

    void on_StartBtn_clicked();

    void on_easyBtn_clicked();

    void on_mediumBtn_clicked();

    void on_hardBtn_clicked();

    void on_lineEdit_editingFinished();

private:
    Ui::StartMenu *ui;
    int difficulty;
    QGraphicsScene *scene;
    QString* playername_;

};

#endif // DIALOG_HH
