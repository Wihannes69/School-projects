/* Tetris
 * ----------
 * TIE-02201/TIE-02207 SPRING 2020
 * ----------
 * Program author
 * Name: Johannes Kalliokoski
 * Student number: 274539
 * UserID: kalliokj
 * E-Mail: johannes.kalliokoski@tuni.fi
 * */
#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QList>
#include <QDebug>
#include <QTimer>
#include <QPointF>
#include <QDebug>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(700,700);
    // We need a graphics scene in which to draw rectangles.
    scene_ = new QGraphicsScene(this);

    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
    ui->graphicsView->setGeometry(left_margin, top_margin,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui->graphicsView->setScene(scene_);

    // The width of the scene_ is BORDER_RIGHT decreased by 1 and
    // the height of it is BORDER_DOWN decreased by 1, because
    // each square of a tetromino is considered to be inside the sceneRect,
    // if its upper left corner is inside the sceneRect.
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    // Setting random engine ready for the first real call.
    int seed = time(0); // You can change seed value for testing purposes
    randomEng.seed(seed);
    distr = std::uniform_int_distribution<int>(0, NUMBER_OF_TETROMINOS - 1);
    distr(randomEng); // Wiping out the first random number (which is almost always 0)
    // After the above settings, you can use randomEng to draw the next falling
    // tetromino by calling: distr(randomEng) in a suitable method.

    // Add more initial settings and connect calls, when needed.
    //Placing some buttons and toggling moving btns. off:

    ui->START_btn->setGeometry(left_margin+BORDER_RIGHT+10, top_margin+BORDER_DOWN-30,
                                     50, 20);
    ui->QUIT_btn->setGeometry(left_margin+BORDER_RIGHT+70, top_margin+BORDER_DOWN-30,
                               50, 20);
    ui->LEFT_btn->setGeometry(left_margin+BORDER_RIGHT+10, top_margin+BORDER_DOWN-100,
                              50, 50);
    ui->RIGHT_btn->setGeometry(left_margin+BORDER_RIGHT+60, top_margin+BORDER_DOWN-100,
                              50, 50);
    ui->SCORE_lbl->setGeometry(left_margin+BORDER_RIGHT+10, top_margin+BORDER_DOWN-300,
                               50, 20);
    ui->SCORE->setGeometry(left_margin+BORDER_RIGHT+70, top_margin+BORDER_DOWN-300,
                           50, 20);
    ui->GOVER_lbl->setGeometry(left_margin+SPAWN-60, top_margin+(BORDER_DOWN/2), 160, 40);

    QFont font = ui->GOVER_lbl->font();
    font.setPointSize(20);
    ui->GOVER_lbl->setFont(font);

    ui->SCORE_lbl->setVisible(false);
    ui->SCORE->setVisible(false);
    ui->GOVER_lbl->setVisible(false);

    btn_lock_(ui->LEFT_btn);
    btn_lock_(ui->RIGHT_btn);

    timer_.setSingleShot(false);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::gameplay);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btn_lock_(QPushButton* button)
{
    // Just making btn. enabling/disabling a bit easier for me maybe..
    if(button->isEnabled())
    {
        button->setEnabled(false);
    }
    else
    {
        button->setEnabled(true);
    }
}

bool MainWindow::can_move(std::string direction)
{
    // Can active tetramino be moved to given direction or not.

    QGraphicsItem* item;
    QTransform trans; //Required by itemAt.

    if(direction == "RIGHT")
    {
        for(unsigned int i = 0; i < 4; ++i)
        {
            if(active_.at(i)->x()+SQUARE_SIDE == BORDER_RIGHT)
            {
                return false;
            }
            item = scene_->itemAt(active_.at(i)->x()+SQUARE_SIDE+1,
                                  active_.at(i)->y()+SQUARE_SIDE, trans);
            if(item != nullptr)
            {

                if(active_.contains(item) == false)
                {
                    return false;
                }
            }
        }

    }

    if(direction == "LEFT")
    {
        for(unsigned int i = 0; i < 4; i++)
        {
            if(active_.at(i)->x() == BORDER_LEFT)
            {
                return false;
            }
            item = scene_->itemAt(active_.at(i)->x()-1,
                                  active_.at(i)->y()+SQUARE_SIDE, trans);
            if(item != nullptr)
            {

                if(active_.contains(item) == false)
                {
                    return false;
                }
            }
        }

    }

    if(direction == "DOWN")
    {
        for(unsigned int i = 0; i < 4; i++)
        {
            if(active_.at(i)->y()+SQUARE_SIDE == BORDER_DOWN)
            {
                return false;
            }

            item = scene_->itemAt(active_.at(i)->x()+1,
                                  active_.at(i)->y()+SQUARE_SIDE+1, trans);
            if(item != nullptr)
            {

                if(active_.contains(item) == false)
                {

                    if(active_.at(i)->y() + SQUARE_SIDE == BORDER_UP)
                    {
                        alive = false;
                    }
                    return false;
                }
            }
        }

    }
    return true;
}

void MainWindow::move(std::string direction)
{
    // Moves active tetramino to given direction.
    if(direction == "RIGHT")
    {
        for(unsigned int i = 0; i < 4; i++)
        {
            active_.at(i)->moveBy(SQUARE_SIDE, 0);
            update();
        }
    }

    if(direction == "LEFT")
    {
        for(unsigned int i = 0; i < 4; i++)
        {
            active_.at(i)->moveBy(-SQUARE_SIDE, 0);
            update();
        }
    }

    if(direction == "DOWN")
    {
        for(unsigned int i = 0; i < 4; i++)
        {
            active_.at(i)->moveBy(0, SQUARE_SIDE);
            update();
        }
    }
}

void MainWindow::gameplay()
{
    // called on each timeout by timer_
    // Moves active tetramino down if possible.
    // Else spawns a new one.
    if(can_move("DOWN"))
    {
        move("DOWN");
    }
    else
    {

        timer_.stop();
        active_.clear();      
        if (alive)
        {
            del_full_rows();
            spawn(true);
            rounds++;
            if (rounds > 15)
            {
                rounds = 15;
            }
            timer_.start(speed - (rounds * 10));
        }
        else
        {
            // Game ended:
            ui->GOVER_lbl->setVisible(true);

            btn_lock_(ui->LEFT_btn);
            btn_lock_(ui->RIGHT_btn);
            btn_lock_(ui->START_btn);

        }
    }
}

void MainWindow::del_full_rows()
{
    /* Deletes full rows from scene, if any.
     * If row(s) are deleted, upper tetramones will move down.
     * Player also receives 100pts per deleted row.
     * Qlist above contains tetramones tht will fall.
     * row contains current rows tetramones.
    */
    QTransform trans; //Required by itemAt.
    QGraphicsItem* item;
    QList<QGraphicsItem*> above;
    QList<QGraphicsItem*> row;
    for (int iy = 0; iy < ROWS; iy++)
    {
        row.clear();
        for (int ix = 0; ix < COLUMNS; ix++)
        {        
            item = scene_->itemAt((ix * SQUARE_SIDE)+1 ,
                                  (iy * SQUARE_SIDE)+1, trans);
            if (item != nullptr)
            {
                row.append(item);
            }
            if (ix == COLUMNS-1)
            {
                if (row.length() == 12)
                {
                    for (int i=0; i < row.length(); i++)
                    {
                        scene_->removeItem(row.at(i));

                    }
                    for (int i=0; i < above.length(); i++)
                    {
                        above.at(i)->moveBy(0, SQUARE_SIDE);
                    }
                    row.clear();
                    update();
                    score = score + 100;
                    ui->SCORE->setText(QString::number(score));
                }
                else
                {
                    for (int i=0; i < row.length(); i++)
                    {
                        above.append(row.at(i));
                    }
                    row.clear();
                }
            }
        }
    }
}

void MainWindow::spawn(bool random, int tetramino_nro)
{
    /* Spawns a tetramino.
     * If random -bool is true, spawned tetramino is randomized.
     * Else parameter int tetramino_ specifys spowned tetramino.
     * Each tetramino consists of four rectangles (first, second, etc.).
     * "active_" is a Qlist consisting of currently active tetraminos four rectangles.
    */

    QPen line(Qt::black);
    QBrush fill(Qt::color0);

    QGraphicsRectItem* first;
    QGraphicsRectItem* second;
    QGraphicsRectItem* third;
    QGraphicsRectItem* fourth;
    if (random)
    {
        tetramino_nro = distr(randomEng);
    }

    fill.setColor(colors.at(tetramino_nro));
    first = scene_->addRect(0, 0, SQUARE_SIDE, SQUARE_SIDE, line, fill);
    second = scene_->addRect(0, 0, SQUARE_SIDE, SQUARE_SIDE, line, fill);
    third = scene_->addRect(0, 0, SQUARE_SIDE, SQUARE_SIDE, line, fill);
    fourth = scene_->addRect(0, 0, SQUARE_SIDE, SQUARE_SIDE, line, fill);

    active_.append(first);
    active_.append(second);
    active_.append(third);
    active_.append(fourth);

    if(Tetromino_kind(tetramino_nro) == HORIZONTAL)
    {
        active_.at(0)->setPos(SPAWN-(2*SQUARE_SIDE), BORDER_UP-2*SQUARE_SIDE);
        active_.at(1)->setPos(SPAWN-SQUARE_SIDE, BORDER_UP-2*SQUARE_SIDE);
        active_.at(2)->setPos(SPAWN, BORDER_UP-2*SQUARE_SIDE);
        active_.at(3)->setPos(SPAWN+SQUARE_SIDE, BORDER_UP-2*SQUARE_SIDE);
    }

    if(Tetromino_kind(tetramino_nro) == LEFT_CORNER)
    {
        active_.at(0)->setPos(SPAWN-SQUARE_SIDE, BORDER_UP-2*SQUARE_SIDE);
        active_.at(1)->setPos(SPAWN-SQUARE_SIDE, BORDER_UP-SQUARE_SIDE);
        active_.at(2)->setPos(SPAWN, BORDER_UP-SQUARE_SIDE);
        active_.at(3)->setPos(SPAWN+SQUARE_SIDE, BORDER_UP-SQUARE_SIDE);
    }

    if(Tetromino_kind(tetramino_nro) == RIGHT_CORNER)
    {
        active_.at(0)->setPos(SPAWN-(2*SQUARE_SIDE), BORDER_UP-SQUARE_SIDE);
        active_.at(1)->setPos(SPAWN-SQUARE_SIDE, BORDER_UP-SQUARE_SIDE);
        active_.at(2)->setPos(SPAWN, BORDER_UP-SQUARE_SIDE);
        active_.at(3)->setPos(SPAWN, BORDER_UP-2*SQUARE_SIDE);
    }

    if(Tetromino_kind(tetramino_nro) == SQUARE)
    {
        active_.at(0)->setPos(SPAWN-SQUARE_SIDE, BORDER_UP-2*SQUARE_SIDE);
        active_.at(1)->setPos(SPAWN-SQUARE_SIDE, BORDER_UP-SQUARE_SIDE);
        active_.at(2)->setPos(SPAWN, BORDER_UP-2*SQUARE_SIDE);
        active_.at(3)->setPos(SPAWN, BORDER_UP-SQUARE_SIDE);
    }

    if(Tetromino_kind(tetramino_nro) == STEP_UP_RIGHT)
    {
        active_.at(0)->setPos(SPAWN-SQUARE_SIDE, BORDER_UP-SQUARE_SIDE);
        active_.at(1)->setPos(SPAWN, BORDER_UP-SQUARE_SIDE);
        active_.at(2)->setPos(SPAWN, BORDER_UP-2*SQUARE_SIDE);
        active_.at(3)->setPos(SPAWN+SQUARE_SIDE, BORDER_UP-2*SQUARE_SIDE);
    }

    if(Tetromino_kind(tetramino_nro) == PYRAMID)
    {
        active_.at(0)->setPos(SPAWN-SQUARE_SIDE , BORDER_UP-SQUARE_SIDE);
        active_.at(1)->setPos(SPAWN, BORDER_UP-SQUARE_SIDE);
        active_.at(2)->setPos(SPAWN, BORDER_UP-2*SQUARE_SIDE);
        active_.at(3)->setPos(SPAWN+SQUARE_SIDE, BORDER_UP-SQUARE_SIDE);
    }

    if(Tetromino_kind(tetramino_nro) == STEP_UP_LEFT)
    {
        active_.at(0)->setPos(SPAWN-SQUARE_SIDE, BORDER_UP-2*SQUARE_SIDE);
        active_.at(1)->setPos(SPAWN, BORDER_UP-2*SQUARE_SIDE);
        active_.at(2)->setPos(SPAWN, BORDER_UP-SQUARE_SIDE);
        active_.at(3)->setPos(SPAWN+SQUARE_SIDE, BORDER_UP-SQUARE_SIDE);
    }

}

void MainWindow::on_START_btn_clicked()
{
    scene_->clear();

    ui->SCORE_lbl->setVisible(true);
    ui->SCORE->setVisible(true);
    ui->GOVER_lbl->setVisible(false);

    btn_lock_(ui->LEFT_btn);
    btn_lock_(ui->RIGHT_btn);
    btn_lock_(ui->START_btn);

    rounds = 0;
    alive = true;
    score = 0;
    timer_.start(speed - rounds * 10);
    ui->SCORE->setText(QString::number(score));
    spawn(true);

}

void MainWindow::on_RIGHT_btn_clicked()
{
    if(can_move("RIGHT"))
    {
        move("RIGHT");
    }
}

void MainWindow::on_LEFT_btn_clicked()
{
    if(can_move("LEFT"))
    {
        move("LEFT");
    }
}

void MainWindow::on_QUIT_btn_clicked()
{
}
