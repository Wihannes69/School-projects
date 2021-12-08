/* Nysse: Player
 * ----------
 * Program authors
 * Name: Johannes Kalliokoski
 * Name: Mika Pajala
 *
 * Defines an unique Player object that can be controlled by the user.
 * Derived from IActor.
 * */
#ifndef PLAYER_HH
#define PLAYER_HH
#include "iplayer.hh"
#include "parameters.h"

#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>


class Player: public IPlayer
{
public:
    Player(Interface::Location, int);
    ~Player();

    /**
     * @brief giveLocation (Inherited) returns actor location
     * @return Actors location.
     */
    Interface::Location giveLocation() const;
    /**
     * @brief move Unused method, inherited from IActor.
     * @param loc
     */

    void move(Interface::Location loc);
    /**
     * @brief move Moves the player location and updates the icon accordingly.
     * @param direction int between 1-4, defines movement direction
     */
    void move(std::vector<bool>);
    /**
     * @brief remove Unused method from IActor.
     */
    void remove();
    /**
     * @brief isRemoved Unused method from IActor.
     * @return always false, player is never removed
     */
    bool isRemoved() const;

    /**
     * @brief takeGraphics Used in initial Player creation,
     *  gives a reference to the Players icon in the scene.
     * @param g Pointer to the PixmapItem referencing the player.
     */
    void takeGraphics(QGraphicsPixmapItem* g);
    /**
     * @brief updateGraphics Updates the icon given to us in takeGraphics
     * with current x,y values.
     */
    void updateGraphics();
    /**
     * @brief canShoot Checks if player has enough ammo to shoot.
     * @return bool
     */
    bool canShoot();

    /**
     * @brief isMoving True if player is currently moving.
     * @return bool
     */
    bool isMoving();

    // need to be public so engine can check them
    int AMMO = 10;
    double health;

    // reference to Player icon in the scene
    QGraphicsPixmapItem* graphicsitem_;

private:
    QPainter painter;
    const int SIZE = 5;
    QRectF rect = QRectF(0,0,SIZE, SIZE);
    Interface::Location location;

};

#endif // PLAYER_HH
