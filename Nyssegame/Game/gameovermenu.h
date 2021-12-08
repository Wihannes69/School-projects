/* Nysse: GameoverMenu
 * ----------
 * Program author
 * Name: Johannes Kalliokoski
 * Name: Mika Pajala
 *
 * Creates a game over menu, displaying statistics
 * using a Statisctics pointer.
 * User can choose to restart or to exit.
 *
 * Inherits QDialog.
 * */

#ifndef GAMEOVERMENU_H
#define GAMEOVERMENU_H

#include <QDialog>
#include <QLabel>
#include "statistics.hh"


namespace Ui {
class GameOverMenu;
}

class GameOverMenu : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief GameOverMenu Creates a new game over dialog.
     * @param gamestats Pointer to the Statistics object storing game data.
     * @param parent Not required.
     */
    explicit GameOverMenu(Statistics* gamestats, QWidget *parent = nullptr);
    ~GameOverMenu();

private slots:
    void on_exitButton_clicked();

    void on_restartButton_clicked();

private:
    Ui::GameOverMenu *ui;
    QLabel statsdisplay_;
};

#endif // GAMEOVERMENU_H
