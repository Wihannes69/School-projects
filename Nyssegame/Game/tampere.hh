/* Nysse: Tampere
 * ----------
 * Program author
 * Name: Johannes Kalliokoski
 * Name: Mika Pajala
 *
 * Game's largest class, derived from ICity.
 * -Spawns and moves actors \Player
 * -Spawns and moves bullets
 * -Handles bullets moving
 * */
#ifndef TAMPERE_HH
#define TAMPERE_HH

#include <QGraphicsScene>
#include <QImage>
#include <QSound>
#include <utility>
#include "interfaces/icity.hh"
#include "mainwindow.hh"
#include "player.hh"
#include "statistics.hh"
#include "parameters.h"
#include <QCloseEvent>
#include <memory>
#include <vector>
#include <QTime>
#include <QDebug>
#include <QGraphicsView>
#include <QPainter>
#include <cmath>
#include <QGraphicsPixmapItem>
#include <QVector>
#include "interfaces/iactor.hh"
#include "interfaces/istop.hh"
#include "iplayer.hh"

class Tampere: public Interface::ICity
{
public:
    Tampere(QGraphicsScene *scene);
    ~Tampere();

    /**
     * @brief setBackground Unused, inherited from Icity.
     * @param basicbackground
     * @param bigbackground
     */
    void setBackground(QImage& basicbackground, QImage& bigbackground);

    /**
     * @brief setClock Sets private clock_ variable. Inherited from Icity.
     * @param clock
     */
    void setClock(QTime clock);

    /**
     * @brief addStop Called for each new stop item.
     * Creates a new QGraphicsRectItem and adds it into the scene_.
     * Also adds the provided stop item into stops_ QVector.
     * @param stop Istop pointer.
     */
    void addStop(std::shared_ptr<Interface::IStop> stop);

    /**
     * @brief startGame Unused method from Icity.
     */
    void startGame();

    /**
     * @brief addActor Adds the actor provided on the scene.
     * Also adds it to the corresponding QList nysses_ or passengers_.
     *
     * Inherited from IActor.
     * @param newactor Shared pointer to the IActor object.
     */
    void addActor(std::shared_ptr<Interface::IActor> newactor);

    /**
     * @brief removeActor Removes the provided actor from the scene
     * and the corresponding QList. Inherited from ICity.
     * @param actor Shared pointer to the IActor object.
     */
    void removeActor(std::shared_ptr<Interface::IActor> actor);

    /**
     * @brief actorRemoved Unused method, inherited from ICity.
     * @param actor
     */
    void actorRemoved(std::shared_ptr<Interface::IActor> actor);

    /**
     * @brief actorExists Checks if provided actor is in either QList
     * nysses_ or passengers_.
     * Inherited from ICity.
     * @param actor Shared pointer to the actor to be searched.
     * @return true if actor is found
     */
    bool findActor(std::shared_ptr<Interface::IActor> actor)const;

    /**
     * @brief actorMoved Updates the provided actor on the scene.
     * @param actor Shared pointer to the actor that was moved.
     */
    void actorMoved(std::shared_ptr<Interface::IActor> actor);

    /**
     * @brief getNearbyActors Provides a vector of nearby actors relative to loc.
     * Actor is nearby if it is closer than NEARBYDISTANCE.
     * @param loc Shared pointer to the Location object that we are comparing to.
     * @return vector of nearby IActors.
     */
    std::vector<std::shared_ptr<Interface::IActor>> getNearbyActors(Interface::Location loc)const;

    /**
     * @brief isGameOver return gamestatus
     * Inherited from ICity.
     * @return gamestatus
     */
    bool isGameOver()const;

    /**
     * @brief shoot Sends out a bullet to the point provided on the map.
     * Sets bulletfired_ as true.
     * @param QPoint pointer to the point where the explosion will occur.
     */
    void shoot(QPoint*);

    /**
     * @brief bulletMove Called by bullettimer_.
     * Moves the bullet object forward
     * or explodes if close enough and sets bulletfired_ to false.
     */
    void bulletMove();

    /**
     * @brief isBullet True if bullet is currently flying.
     * @return bulletfired_
     */
    bool isBullet();

    /**
     * @brief explosion Creates an explosion at point provided
     * and destroys objects that are nearby defined by Tampere::getNearbyActors.
     * @param QPointF point where explosion should occur.
     */
    void explosion(QPointF);

    /**
     * @brief hasExploded helper method for bullet moving
     * @return exploded_
     */
    bool hasExploded();

    /**
     * @brief setUnexploded Sets exploded_ to false. Needed for engine::updateBullet.
     */
    void setUnexploded();

    QGraphicsPixmapItem *explosive;

    /**
     * @brief takeDmg Returns the amount of damage that player should take
     *  based on nearby passengers.
     * @return damage as double
     */
    double takeDmg();

    /**
     * @brief clear Clears scene when game is restarted.
     */
    void clear();

    /**
     * @brief takeLists Gives Tampere pointers to the nysses_ and passengers_ QLists.
     */
    void takeLists(QList<std::pair<std::shared_ptr<Interface::IActor>, QGraphicsPixmapItem*>>*
, QList<std::pair<std::shared_ptr<Interface::IActor>, QGraphicsPixmapItem*>>*);

    /**
     * @brief giveStats Returns pointer to Statistics object that Tampere has collected.
     * @return Statistic pointer.
     */
    Statistics* giveStats();


public slots:
    /**
     * @brief spawnPlayer Creates a PixmapItem for the provided
     * Player object and adds it to the scene.
     */
    void spawnPlayer(Player*);

private:
    QList<std::pair<std::shared_ptr<Interface::IActor>, QGraphicsPixmapItem*>> *nysses_;

    QList<std::pair<std::shared_ptr<Interface::IActor>, QGraphicsPixmapItem*>> *passengers_;

    QGraphicsScene* scene_;

    std::pair<std::shared_ptr<Interface::IActor>, QGraphicsPixmapItem*> pair;

    int type(std::shared_ptr<Interface::IActor>);

    Statistics stats_;

    QVector<QGraphicsRectItem*> stops_;

    void closeEvent(QCloseEvent*);

    bool isInMap(QPointF);
    QPixmap BusLogoR;
    QPixmap BusLogoL;
    QPixmap PassengerLogo;
    QPixmap player_img1;
    QPixmap player_img2;
    QPixmap explosion_img;
    QGraphicsPixmapItem *gamer_;
    QGraphicsEllipseItem* bullet_;
    float xTravelStep;
    float yTravelStep;
    QPoint *destination;

    QSound *takata;
    QTime clock_;

    bool gameEnded = false;
    bool bulletfired_ = false;
    bool exploded_ = false;

};



#endif // TAMPERE_HH
