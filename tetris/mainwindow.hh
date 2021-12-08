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
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH
#include <QMainWindow>
#include <QGraphicsScene>
#include <QPushButton>
#include <random>
#include <QColor>
#include <QList>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_START_btn_clicked();

    void on_RIGHT_btn_clicked();

    void on_LEFT_btn_clicked();

    void on_QUIT_btn_clicked();

private:
    Ui::MainWindow *ui;

    QGraphicsScene* scene_;

    QTimer timer_;

    bool alive;
    bool can_move(std::string direction);

    // Toggles button locked or "open"
    void btn_lock_(QPushButton* button);

    // Moves active tetramino:
    void move(std::string direction);

    // Controls the game, triggered after each timeout.
    void gameplay();

    // Deletes full rows, if any.
    void del_full_rows();

    QList<QGraphicsItem*> active_; // Current tetramino

    // Spawn tetramino:
    void spawn(bool random, int tetramino_nro = 0);


    int rounds;
    const int speed = 300;
    int score; // Current score

    // Constants describing scene coordinates
    const int left_margin = 100; // x coordinate
    const int top_margin = 100; // y coordinate
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 480; // 260; (in moving circle)
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 240; // 680; (in moving circle)
    const int SPAWN = BORDER_RIGHT / 2;
    // Size of a tetromino component
    const int SQUARE_SIDE = 20;
    // Number of horizontal cells (places for tetromino components)
    const int COLUMNS = BORDER_RIGHT / SQUARE_SIDE;
    // Number of vertical cells (places for tetromino components)
    const int ROWS = BORDER_DOWN / SQUARE_SIDE;

    // Constants for different tetrominos and the number of them
    enum Tetromino_kind {HORIZONTAL,
                         LEFT_CORNER,
                         RIGHT_CORNER,
                         SQUARE,
                         STEP_UP_RIGHT,
                         PYRAMID,
                         STEP_UP_LEFT,
                         NUMBER_OF_TETROMINOS};
    // Color of each diff. tetramino:
    QList<QColor> colors = {
        Qt::cyan, Qt::blue, "orange", Qt::yellow, Qt::green, Qt::magenta, Qt::red
    };

    // For randomly selecting the next dropping tetromino
    std::default_random_engine randomEng;
    std::uniform_int_distribution<int> distr;

};

#endif // MAINWINDOW_HH
