/* Nysse: Engine
 * ----------
 * Program author
 * Name: Johannes Kalliokoski
 * Name: Mika Pajala
 *
 * Core engine of the game, first thing called and last thing running.
 * Creates MainWindow and StartDialog.
 * Creates a Tampere to handle city logic.
 * */

#ifndef ENGINE_HH
#define ENGINE_HH
#include <fstream>
#include <QString>
#include <vector>
#include "actors/nysse.hh"
#include "interfaces/icity.hh"
#include "../Course/CourseLib/core/logic.hh"
#include "tampere.hh"
#include "mainwindow.hh"
#include "startmenu.hh"
#include "player.hh"
#include "gameovermenu.h"
#include "parameters.h"

/**
 * @brief The Engine class provides oversight on the entire game application,
 * creating windows and the city class.
 */
class Engine: public QObject

{
    Q_OBJECT;
public:
    Engine();
    virtual ~Engine();
    /**
     * @brief gameSetup Called when program starts. Sets up logic
     * and presents startdialog.
     * @param a : pointer to QApplication hosting the game.
     * If not null, game is starting for the first time.
     * @return always returns true
     */
    int gameSetup(QApplication *a = nullptr);

public slots:
    /**
     * @brief EngineStart Called when start button is pressed.
     * Starts logic and necessary timers for player movement.
     */
    void EngineStart();
    /**
     * @brief setDifficulty Sets spawning location and player health based
     * on provided difficulty level (1-3).
     */
    void setDifficulty(int);

    /**
     * @brief MovePlayer Calls player items move() based on the vector.
     * @param 4-item vector specifying directions.
     */
    void MovePlayer(std::vector<bool>);

    /**
     * @brief Shoot Sends out a bullet if conditions allow.
     * @param QPoint object stating where the bullet should land.
     */
    void Shoot(QPoint);

    /**
     * @brief updateBullet Moves bullet forward based on timer tick.
     */
    void updateBullet();

    /**
     * @brief pauseGame Brings up PauseMenu.
     */

    void gameRestart();

private:
    MainWindow m_;
    Tampere tampere_;
    CourseSide::Logic * logic_;
    Player* gamer_;
    QApplication *a_;
    QList<std::pair<std::shared_ptr<Interface::IActor>, QGraphicsPixmapItem*>> nysses_;
    QList<std::pair<std::shared_ptr<Interface::IActor>, QGraphicsPixmapItem*>> passengers_;
    QPoint *point_;
    QTimer *bullettimer_;
    QTimer *dmgtimer_;
    bool playermoving;
    bool app_executed = false;
    void takeDmg();
    void gameover();   
    QList <std::pair<int, std::string>> topscore;
    QString* playername = new QString("Anonymous");
    std::ifstream file;
    std::vector<std::string> split(std::string&, char);
    void saveGamestats();
    bool readFile();
};




#endif // ENGINE_HH
