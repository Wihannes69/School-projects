/* Nysse: MainWindow
 * ----------
 * Program author
 * Name: Johannes Kalliokoski
 * Name: Mika Pajala
 *
 * The main window of the game. Handles UI and
 * displaying the game over -window.
 * */
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <QApplication>
#include <QGraphicsView>

#include <QPixmap>
#include <QImage>
#include <QGraphicsEllipseItem>
#include <QGraphicsProxyWidget>
#include <QPainter>
#include <QCursor>
#include <QSound>

#include <QPushButton>
#include <QLCDNumber>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QLabel>
#include <QTimer>

#include <QDebug>


#include "actors/nysse.hh"
#include "gameovermenu.h"
#include "pausemenu.hh"
#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QGraphicsView* view_;
    QGraphicsScene* scene_;

    void setHP(int);

    bool hasGameEnded(Statistics*);

    void setHealthMax(int);

    void shooted(int);

signals:
    void startGame();
    void pauseGame();
    void restart();
    void shoot(QPoint);
    void setClock();
    void playerMoved(std::vector<bool>);


private slots:

    void on_StartBtn_clicked();

    void on_PauseBtn_clicked();

    void checkPlayerMove();

private:
    Ui::MainWindow *ui;
    QImage image_;

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void resetMovement();
    void mousePressEvent(QMouseEvent *event);
    void mouseWheelEvent(QMouseEvent *event);

    bool eventFilter(QObject *object, QEvent *event);

    QTimer movetimer_;
    bool pmoveup;
    bool pmoveleft;
    bool pmovedown;
    bool pmoveright;



};

#endif // MAINWINDOW_HH
