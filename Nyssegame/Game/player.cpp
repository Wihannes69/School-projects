#include "player.hh"

Player::Player(Interface::Location loc, int inithealth)
{
    location = loc;
    health = inithealth;

}

Player::~Player()
{

}

Interface::Location Player::giveLocation() const
{
    return location;
}

void Player::move(Interface::Location loc)
{
    loc.giveX();
}

void Player::move(std::vector<bool> directions)
{

    int x = location.giveX();
    int y = location.giveY();
    int newx = x;
    int newy = y;

    if(directions.at(UP-1))
    {
        if(y < UPLIMIT){
            newy += 10;
        }
    }
    if(directions.at(LEFT-1))
    {
        if(x > LEFTLIMIT){
            newx -= 10;
        }
    }
    if(directions.at(RIGHT-1))
    {
        if(x < RIGHTLIMIT){
            newx += 10;
        }
    }
    if(directions.at(DOWN-1))
    {
        if(y > DOWNLIMIT){
            newy -= 10;
        }
    }
    location.setXY(newx,newy);
    updateGraphics();
}

bool Player::isRemoved() const
{
    return false; //Inherited, is never true
}

void Player::takeGraphics(QGraphicsPixmapItem *g)
{
    graphicsitem_ = g;
}

void Player::updateGraphics()
{
    double x =(((location.giveEasternCoord() - WEST) / (EAST - WEST)) * WIDTH);
    double y = (((NORTH - location.giveNorthernCoord()) / (NORTH - SOUTH)) * HEIGHT);
    graphicsitem_->setPos(x,y);
}

bool Player::canShoot()
{
    if ( AMMO > 0){
        return true;
    }
    else{
        return false;
    }
}

void Player::remove()
{

}



