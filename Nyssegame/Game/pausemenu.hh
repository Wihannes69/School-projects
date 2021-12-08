/* Nysse: PauseMenu
 * ----------
 * Program author
 * Name: Johannes Kalliokoski
 * Name: Mika Pajala
 *
 * Dialog from MainWindow, continue playing or exit to StartMenu.
 * */
#ifndef PAUSEMENU_HH
#define PAUSEMENU_HH

#include <QDialog>
#include "startmenu.hh"

namespace Ui {
class PauseMenu;
}

class PauseMenu : public QDialog
{
    Q_OBJECT

public:
    explicit PauseMenu(QWidget *parent = nullptr);
    ~PauseMenu();

private slots:
    void on_StartBtn_clicked();

    void on_ExitBtn_clicked();

private:
    Ui::PauseMenu *ui;
};

#endif // PAUSEMENU_HH
