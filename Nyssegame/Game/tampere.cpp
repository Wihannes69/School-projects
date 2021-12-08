#include "tampere.hh"



Tampere::Tampere(QGraphicsScene *scene):
    Interface::ICity(),
    scene_(scene){
    scene_->setItemIndexMethod(QGraphicsScene::NoIndex);

    BusLogoR = QPixmap("../../codecookingkitchen/symbols/nysse_.png");
    QImage imgL = QImage("../../codecookingkitchen/symbols/nysse_.png").mirrored(true, false);
    BusLogoL.convertFromImage(imgL);
    PassengerLogo = QPixmap("../../codecookingkitchen/symbols/zombie.png");
    player_img1 = QPixmap("../../codecookingkitchen/symbols/copter.png");
    QImage img = QImage("../../codecookingkitchen/symbols/copter.png").mirrored(true, false);
    player_img2.convertFromImage(img);
    explosion_img = QPixmap("../../codecookingkitchen/symbols/boom.png");
}

Tampere::~Tampere(){

}

void Tampere::setBackground(QImage& basicbackground, QImage& bigbackground){
    QImage temp = basicbackground;
    temp = bigbackground;
}

void Tampere::setClock(QTime clock){

//    qDebug() << "Tampere::setClock to " << clock.hour() << clock.minute();
    clock_ = clock;
//    qDebug() << "time received: " << clock_.hour() << clock_.minute();
}

void Tampere::addStop(std::shared_ptr<Interface::IStop> stop){

    Interface::Location loc = stop->getLocation();

    double x =(((loc.giveEasternCoord() - WEST) / (EAST - WEST)) * WIDTH);
    double y = (((NORTH - loc.giveNorthernCoord()) / (NORTH - SOUTH)) * HEIGHT);


    if (Tampere::isInMap(QPointF(x,y))){
        QGraphicsRectItem* rect_ = new QGraphicsRectItem(x-2.5, y-2.5, 5,5);
        rect_->setBrush(QBrush(Qt::blue));
        rect_->setOpacity(0.5);
        scene_->addItem(rect_);
        scene_->update(x-2.5, y-2.5, 5,5);

        stops_.push_back(rect_);
    }

}

void Tampere::startGame(){

}

void Tampere::addActor(std::shared_ptr<Interface::IActor> newactor){

    Interface::Location loc = newactor->giveLocation();
    double x =(((loc.giveEasternCoord() - WEST) / (EAST - WEST)) * WIDTH);
    double y = (((NORTH - loc.giveNorthernCoord()) / (NORTH - SOUTH)) * HEIGHT);



    if (Tampere::type(newactor) == NYSSE){
        QGraphicsPixmapItem *bus_ = new QGraphicsPixmapItem();

        bus_->setZValue(5);
        bus_->setPixmap(BusLogoR.scaled(BUS_SIZE,BUS_SIZE));


        //Only draw if the bus is at map:
        if (Tampere::isInMap(QPointF(x,y))){

            scene_->addItem(bus_);
            bus_->setPos(x-BUS_SIZE/2, y-BUS_SIZE/2);
             // Fixes odd placement caused by bad texturing by me...

        }
        pair.first = newactor;
        pair.second = bus_;
        nysses_->push_back(pair);
        scene_->update(x-BUS_SIZE/2, y-BUS_SIZE/2, BUS_SIZE, BUS_SIZE);
        qDebug()<< "bus added!" << x << y;
        stats_.newNysse();


    }

    if (Tampere::type(newactor) == PASSENGER){
        QGraphicsPixmapItem *passenger_ = new QGraphicsPixmapItem();

        passenger_->setZValue(4);
        passenger_->setPixmap(PassengerLogo.scaled(PASS_SIZE,PASS_SIZE));
        passenger_->setPos(x-PASS_SIZE/2,y-PASS_SIZE/2);

        //Only draw if the passenger is at map:
        if (Tampere::isInMap(QPointF(x,y))){
            scene_->addItem(passenger_);


        }
        pair.first = newactor;
        pair.second = passenger_;

        passengers_->push_back(pair);
        scene_->update(x-PASS_SIZE/2, y-PASS_SIZE/2, PASS_SIZE, PASS_SIZE);
    }


}

void Tampere::removeActor(std::shared_ptr<Interface::IActor> actor){
    //qDebug() << nysses_.size() << passengers_.size(); //good
    if (Tampere::type(actor) == NYSSE){
        for (QList<std::pair<std::shared_ptr<Interface::IActor>, QGraphicsPixmapItem*>>::iterator it = nysses_->begin();
             it != nysses_->end(); ++it){
            if (it->first == actor){
                if (it->second->scene() != 0){
                    it->second->scene()->removeItem(it->second);
                    scene_->update(it->second->pos().x()-BUS_SIZE/2, it->second->pos().y()-BUS_SIZE/2, BUS_SIZE,BUS_SIZE);
                }
                it->first = nullptr;
                it->second = nullptr;
                nysses_->erase(it);
                break;
            }
        }
    }
    if (Tampere::type(actor) == PASSENGER){
        for (QList<std::pair<std::shared_ptr<Interface::IActor>,QGraphicsPixmapItem*>>::iterator it = passengers_->begin();
             it != passengers_->end(); ++it){
            if (it->first == actor){
                if (it->second->scene() != 0){
                    it->second->scene()->removeItem(it->second);
                    scene_->update(it->second->pos().x()-PASS_SIZE/2, it->second->pos().y()-PASS_SIZE/2, PASS_SIZE,PASS_SIZE);
                }
                it->first = nullptr;
                it->second = nullptr;
                passengers_->erase(it);

                break;
            }
        }
    }
    if (Tampere::type(actor) == PLAYER){
        scene_->removeItem(gamer_);
        scene_->update(gamer_->pos().x()-PLAYER_SIZE/2, gamer_->pos().y()-PLAYER_SIZE/2, PLAYER_SIZE,PLAYER_SIZE);
    }

}

void Tampere::actorRemoved(std::shared_ptr<Interface::IActor> actor){
    std::shared_ptr<Interface::IActor> temp = actor;
}

bool Tampere::findActor(std::shared_ptr<Interface::IActor> actor)const{
    for (QList<std::pair<std::shared_ptr<Interface::IActor>, QGraphicsPixmapItem*>>::const_iterator it = nysses_->begin();
         it != nysses_->end(); ++it){
        if (it->first == actor){
            return true;
        }
    }
    for (QList<std::pair<std::shared_ptr<Interface::IActor>,QGraphicsPixmapItem*>>::const_iterator it = passengers_->begin();
         it != passengers_->end(); ++it){
        if (it->first == actor){
            return true;
        }
    }
    return false;
}

void Tampere::actorMoved(std::shared_ptr<Interface::IActor> actor){

    if (Tampere::type(actor) == NYSSE){
        for (QList<std::pair<std::shared_ptr<Interface::IActor>, QGraphicsPixmapItem*>>::iterator it = nysses_->begin();
             it != nysses_->end(); ++it){
            if (it->first == actor){
                Interface::Location loc = actor->giveLocation();
                double x =(((loc.giveEasternCoord() - WEST) / (EAST - WEST)) * WIDTH);
                double y = (((NORTH - loc.giveNorthernCoord()) / (NORTH - SOUTH)) * HEIGHT);

                //Bus is trying to move to x,y which is in map:
                if (Tampere::isInMap(QPointF(x,y))){


                    //Moving to left:
                    if (x-(it->second->scenePos().rx()) < 0){
                        it->second->setPixmap(BusLogoL.scaled(BUS_SIZE,BUS_SIZE));
                    }
                    //Moving to right:
                    if (x-(it->second->scenePos().rx()) > 0){
                        it->second->setPixmap(BusLogoR.scaled(BUS_SIZE,BUS_SIZE));
                    }

                    it->second->moveBy(x-(it->second->scenePos().rx()), y-(it->second->scenePos().ry()));
                    scene_->update();
                    it->second->setOffset(-25,-30); // FIXING placement issue
                    //Currently not in map:
                    if(it->second->scene() != scene_ /* means there is no scene*/){
                        scene_->addItem(it->second);

                    }
                }
                //Moving out of bounds:
                else if (not Tampere::isInMap(QPointF(x,y))){
                    //currently in map:
                    if (it->second->scene() == scene_){
                        //No delete yet, the bus might come back to map...
                        scene_->removeItem(it->second);

                    }
                    scene_->update(x-BUS_SIZE/2, y-BUS_SIZE/2, BUS_SIZE, BUS_SIZE);
                }
                break;
            }
        }
    }
    if (Tampere::type(actor) == PASSENGER){
        std::shared_ptr<CourseSide::Passenger> passenger = std::dynamic_pointer_cast<CourseSide::Passenger>(actor);
        if(passenger->isInVehicle()){

        }
        else{
            for (QList<std::pair<std::shared_ptr<Interface::IActor>,QGraphicsPixmapItem*>>::iterator it = passengers_->begin();
                 it != passengers_->end(); ++it){
                if (it->first == actor){
                    Interface::Location loc = actor->giveLocation();
                    double x =(((loc.giveEasternCoord() - WEST) / (EAST - WEST)) * WIDTH);
                    double y = (((NORTH - loc.giveNorthernCoord()) / (NORTH - SOUTH)) * HEIGHT);
                    if (Tampere::isInMap(QPointF(x,y))){

                        it->second->moveBy(x-(it->second->scenePos().rx()), y-(it->second->scenePos().ry()));
                        scene_->update();
                        if(it->second->scene() != scene_ /* means there is no scene*/){
                            scene_->addItem(it->second);
                        }
                    }
                    else if (not Tampere::isInMap(QPointF(x,y))){
                        //currently in map:
                        if (it->second->scene() == scene_){
                            //No delete yet, the bus might come back to map...
                            scene_->removeItem(it->second);

                        }
                        scene_->update(x-PASS_SIZE/2, y-PASS_SIZE/2, PASS_SIZE, PASS_SIZE);

                    }
                    break;
                }
            }
        }

    }
}

std::vector<std::shared_ptr<Interface::IActor>> Tampere::getNearbyActors(Interface::Location loc)const{
    std::vector<std::shared_ptr<Interface::IActor>> vector;
    for (QList<std::pair<std::shared_ptr<Interface::IActor>, QGraphicsPixmapItem*>>::const_iterator it = nysses_->begin(); it < nysses_->end(); it++){
        if (it->first->giveLocation().isClose(loc, NEARBYDISTANCE)){
            vector.push_back(it->first);

        }
    }
    for (QList<std::pair<std::shared_ptr<Interface::IActor>,QGraphicsPixmapItem*>>::const_iterator it = passengers_->begin(); it != passengers_->end(); it++){
        if (it->first->giveLocation().isClose(loc, NEARBYDISTANCE)){
            vector.push_back(it->first);
        }
    }
    return vector;
}

bool Tampere::isGameOver()const{
    return gameEnded;
}

void Tampere::shoot(QPoint* point)
{
    xTravelStep = 0;
    yTravelStep = 0;
    destination = new QPoint();
    bullet_ = new QGraphicsEllipseItem(0, 0, 10,10);

    bullet_->setBrush(QColor(0,0,0));
    scene_->addItem(bullet_);

    destination->setX(point->x() - 53);
    destination->setY(point->y() - 53);

    if(destination->x() < gamer_->pos().x()){
        gamer_->setPixmap(player_img2.scaled(100,66));
        bullet_->setPos(gamer_->pos().x(), gamer_->pos().y()+30);
    }
    else{
        gamer_->setPixmap(player_img1.scaled(100,66));
        bullet_->setPos(gamer_->pos().x()+100, gamer_->pos().y()+30);
    }


    float xTravel = destination->x() - bullet_->pos().x();
    float yTravel = destination->y() - bullet_->pos().y();
    float xyTravel = sqrt(pow(xTravel, 2)+pow(yTravel, 2));

    QSound::play("../../codecookingkitchen/noices/pam_1.wav");
    if(xyTravel < 50){
        xTravelStep = xTravel / 2;
        yTravelStep = yTravel / 2;
    }
    else if (xyTravel < 100){
        xTravelStep = xTravel / 4;
        yTravelStep = yTravel / 4;
    }
    else if (xyTravel < 200){
        xTravelStep = xTravel / 10;
        yTravelStep = yTravel / 10;
    }
    else if (xyTravel < 400){
        xTravelStep = xTravel / 15;
        yTravelStep = yTravel / 15;
    }
    else{
        xTravelStep = xTravel / 25;
        yTravelStep = yTravel / 25;
    }
    scene_->update(bullet_->pos().x(), destination->y()-5 - bullet_->pos().y()-5, 10,10);
    bulletfired_ = true;
}

void Tampere::bulletMove()
{
    bullet_->moveBy(xTravelStep, yTravelStep);
    scene_->update(bullet_->pos().x(), destination->y()-5 - bullet_->pos().y()-5, 10,10);
    //Bullet out of bounds:
    if(std::abs(bullet_->pos().rx() - gamer_->pos().rx()) > WIDTH / 2 or
            std::abs(bullet_->pos().ry() - gamer_->pos().ry()) > HEIGHT / 2){

        bulletfired_ = false;
        delete destination;
    }

    if (std::abs(bullet_->x() - destination->x()) < 15 and
           std::abs(bullet_->y() - destination->y()) < 15){
        QSound::play("../../codecookingkitchen/noices/pam_2.wav");
    }
    if (std::abs(bullet_->x() - destination->x()) < 10 and
           std::abs(bullet_->y() - destination->y()) < 10){
        Tampere::explosion(bullet_->pos());

        bulletfired_ = false;
        delete destination;
    }


}

bool Tampere::isBullet()
{
    return bulletfired_;
}

void Tampere::explosion(QPointF point)
{
    //Some Math:
    int east = (((point.x()) / WIDTH) * (EAST - WEST)) + WEST;
    int north = ((std::abs(point.y() - HEIGHT) / HEIGHT) * (NORTH -SOUTH)) + SOUTH;

    std::vector<std::shared_ptr<Interface::IActor>> victims;
    victims = Tampere::getNearbyActors(Interface::Location(north, east));

    explosive = new QGraphicsPixmapItem();
    delete bullet_;
    explosive->setZValue(8); // make it topmost
    explosive->setPos(point.x()-50, point.y()-50);
    explosive->setPixmap(explosion_img.scaled(100,100));
    scene_->addItem(explosive);
    scene_->update(explosive->x()-50, explosive->y()-50, 100,100);

    for (unsigned int i = 0; i < victims.size(); i++){
        //Kill the passengers:
        if (Tampere::type(victims.at(i)) == NYSSE){
            std::shared_ptr<CourseSide::Nysse> bus = std::dynamic_pointer_cast<CourseSide::Nysse>(victims.at(i));
            for (unsigned int i_ = 0; i_ < bus->getPassengers().size(); i_++){
                Tampere::removeActor(bus->getPassengers().at(i_));
                //killedPeople++;
                stats_.passengerRemoved();

            }
            stats_.nysseRemoved();
            Tampere::removeActor(victims.at(i));
        }
        else{
            stats_.passengerRemoved();
            Tampere::removeActor(victims.at(i));
        }
    }
        Tampere::exploded_ = true;
        scene_->update(explosive->x()-50, explosive->y()-50, 100,100);
}

bool Tampere::hasExploded()
{
    return exploded_;
}

void Tampere::setUnexploded()
{
    exploded_ = false;
}

double Tampere::takeDmg()
{
    double total_dmg = 0;
    double damagers = 0;

    for (int i = 0; i < passengers_->size(); i++){
        double dx = std::abs(gamer_->pos().x()+30 - passengers_->at(i).second->pos().rx());
        double dy = std::abs(gamer_->pos().y()+15 - passengers_->at(i).second->pos().ry());
        if( (dx < 30) and (dy < 30))
        {
            damagers++;
        }
    }
    if(Tampere::exploded_){
        if (std::abs(explosive->pos().x() - gamer_->pos().x()) < 50 and
                std::abs(explosive->pos().y() - gamer_->pos().y()) < 50){
            damagers = damagers + 25;
        }
    }

    total_dmg = damagers / 5;
    return total_dmg;
}


void Tampere::clear()
{
    //Remove everything from the scene; needed when game is restarted (startmenu -> game ->pausemenu -> startmenu -> game)
    for (int i = 0; i < stops_.size(); i++){
        delete stops_.at(i);
    }
    for (int i = 0; i < nysses_->size(); i++){
        delete nysses_->at(i).second;
    }
    for (int i = 0; i < passengers_->size(); i++){
        delete passengers_->at(i).second;
    }

    stops_.clear();
    nysses_->clear();
    passengers_->clear();
    scene_->clear();
    takata->stop();

}

void Tampere::takeLists(QList<std::pair<std::shared_ptr<Interface::IActor>, QGraphicsPixmapItem *> >* n, QList<std::pair<std::shared_ptr<Interface::IActor>, QGraphicsPixmapItem *> >* p)
{
    nysses_ = n;
    passengers_ = p;
}

Statistics* Tampere::giveStats()
{
    return &stats_;
}


void Tampere::spawnPlayer(Player *player)
{
    Interface::Location loc = player->giveLocation();
    double x =(((loc.giveEasternCoord() - WEST) / (EAST - WEST)) * WIDTH);
    double y = (((NORTH - loc.giveNorthernCoord()) / (NORTH - SOUTH)) * HEIGHT);
    gamer_ = new QGraphicsPixmapItem();
    player->takeGraphics(gamer_);

    gamer_->setZValue(10);
    gamer_->setPixmap(player_img1.scaled(3*PLAYER_SIZE,2*PLAYER_SIZE));
    gamer_->setPos(x,y);
    scene_->addItem(gamer_);

    takata = new QSound("../../codecookingkitchen/noices/takataka.wav");
    takata->setLoops(QSound::Infinite);
    takata->play();
    qDebug() << "Tampere: player location is (x,y)" << player->giveLocation().giveX() << player->giveLocation().giveY();

}

int Tampere::type(std::shared_ptr<Interface::IActor> actor)
{
    std::shared_ptr<Interface::IVehicle> isBus = std::dynamic_pointer_cast<Interface::IVehicle>(actor);
    std::shared_ptr<Interface::IPassenger> isPassenger = std::dynamic_pointer_cast<Interface::IPassenger>(actor);

    if (isBus){
        return NYSSE;
    }
    else if (isPassenger){
        return PASSENGER;
    }
    std::shared_ptr<IPlayer> isPlayer = std::dynamic_pointer_cast<IPlayer>(actor);
    if (isPlayer){
        return PLAYER;
    }
    return 0;

}

void Tampere::closeEvent(QCloseEvent* event)
{
    Tampere::clear();
    event->accept();
}

bool Tampere::isInMap(QPointF point)
{
    double x = point.rx();
    double y = point.ry();
    if (x <= WIDTH && y <= HEIGHT && x >= 0 && y >=0){
        return true;
    }
    else{
        return false;
    }
}




