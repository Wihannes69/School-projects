#include "engine.hh"
Engine::Engine():
    tampere_(m_.scene_)
{
    tampere_.takeLists(&nysses_, &passengers_);

    playermoving = false;
    app_executed = false;

    bullettimer_ = new QTimer();
    dmgtimer_ = new QTimer();

    logic_ = new CourseSide::Logic(this);

    connect(bullettimer_, &QTimer::timeout,
            this, &Engine::updateBullet);

    connect(dmgtimer_, &QTimer::timeout,
            this, &Engine::takeDmg);

    connect(&m_, &MainWindow::startGame,
            this, &Engine::EngineStart);

    connect(&m_, &MainWindow::playerMoved,
            this, &Engine::MovePlayer);

    connect(&m_, &MainWindow::restart,
            this, &Engine::gameRestart);

}

Engine::~Engine()
{
    delete bullettimer_;
    delete dmgtimer_;
    delete playername;
    delete logic_;
}

void Engine::EngineStart()
{
    logic_->finalizeGameStart();
    tampere_.spawnPlayer(gamer_);

    bullettimer_->start(BULLETTIME);
    dmgtimer_->start(DAMAGETIME);

    connect(&m_, SIGNAL(shoot(QPoint)),
            this, SLOT(Shoot(QPoint)));
    m_.view_->centerOn(gamer_->graphicsitem_->pos());
    qDebug() << "engine started";

}

int Engine::gameSetup(QApplication *a)
{
    if(a != nullptr){
        //First time
        a_ = a;
    }
    else{
        //Exited game:
        delete logic_;
        logic_ = new CourseSide::Logic(this);

    }

    Engine::readFile();

    StartMenu *startdialog_ = new StartMenu(playername, &topscore);

    connect(startdialog_, &StartMenu::Difficulty,
            this, &Engine::setDifficulty);

    // open start dialog
    startdialog_->exec();

    if (startdialog_->result() == 1){
        //Game started
        m_.show();
    }
    else if(startdialog_->result() == 0){
        //Game closed
        if (app_executed){
            a_->quit();

        }
        else{

            return 0;
        }
    }

    delete startdialog_;
    std::shared_ptr<Interface::ICity> tampere_ptr;
    tampere_ptr = std::make_shared<Tampere>(tampere_);

    if(logic_->takeCity(tampere_ptr))
    {
        qDebug() << "city saved to logic!";

    }
    // adds stops and busses to game logic
    logic_->fileConfig();

    if(!app_executed){
    // this only occurs at the initial run
        qDebug() << "initally starting game!";
        app_executed = true;
        a_->exec();

    }
    return 0;
}


void Engine::setDifficulty(int i)
{

    if (i == EASY){
        // set to easy
        gamer_ = new Player(EASYLOCATION,EASYHEALTH);
        m_.setHealthMax(EASYHEALTH);
    }
    else if(i == MEDIUM){
        // set to medium
        gamer_ = new Player(MEDIUMLOCATION, MEDIUMHEALTH);
        m_.setHealthMax(MEDIUMHEALTH);
    }
    else if(i == HARD){
        // set to hard
        gamer_ = new Player(HARDLOCATION, HARDHEALTH);
        m_.setHealthMax(HARDHEALTH);
    }
}

void Engine::MovePlayer(std::vector<bool> directions)
{
    gamer_->move(directions); //change internal coordinates

    m_.view_->centerOn(gamer_->graphicsitem_->pos());
}

void Engine::Shoot(QPoint pnt)
{
    if(gamer_->canShoot() and not tampere_.isBullet() and not tampere_.hasExploded()){
        gamer_->AMMO = gamer_->AMMO - 1;
        point_ = &pnt;
        tampere_.shoot(point_);
        m_.shooted(gamer_->AMMO);
    }
}

void Engine::updateBullet()
{

    //This stuff happens every 0,1s
    //Can be nice for other stuff too
    if (tampere_.isBullet())
        tampere_.bulletMove();
    else if (tampere_.hasExploded()){
        delete tampere_.explosive;
        tampere_.setUnexploded();
    }
}

void Engine::takeDmg()
{
    double amount = tampere_.takeDmg();
    if (gamer_->health <= amount){
        //DIE
        m_.setHP(amount);
        saveGamestats();
        tampere_.clear();

        bullettimer_->stop();
        dmgtimer_->stop();

        disconnect(&m_, SIGNAL(playerMoved(std::vector<bool>)),
                this, SLOT(MovePlayer(std::vector<bool>)));

        disconnect(&m_, SIGNAL(shoot(QPoint)),
                this, SLOT(Shoot(QPoint)));


        int i = m_.hasGameEnded(tampere_.giveStats());
        if (i == 1){
            a_->exit();
        }
    }
    else if (amount > HEALTHTRESHOLD){
        gamer_->health = gamer_->health - amount;
        qDebug() << "health is " << gamer_->health;
        m_.setHP(gamer_->health);
        QSound::play("../../codecookingkitchen/noices/au_1.wav");
    }
}

bool Engine::readFile()
{

    topscore.clear();
    file = std::ifstream(FILELOC);
    if ( not file) {
        qDebug() << "Error! The file cannot be opened.";
        return EXIT_FAILURE;
    } else {
        std::pair<int, std::string> pair;
        std::string row;
        while ( getline(file, row) ) {

            std::vector<std::string> fields = Engine::split(row, ';');
            if(fields.size() == 3){
                std::string name = fields.at(0);
//                qDebug() << QString::fromStdString(fields.at(0)+fields.at(1)+fields.at(2));
                pair.first = std::stoi(fields[1])*2 + std::stoi(fields[2]);
                pair.second = name + " - Nysses: " + fields[1] + ";  Passengers: " + fields[2];
            }
            topscore.append(pair);
        }

        file.close();
        std::sort(topscore.begin(), topscore.end());
        std::reverse(topscore.begin(), topscore.end());
        return EXIT_SUCCESS;
    }


}

void Engine::gameRestart()
{
    tampere_.clear();
    gameSetup(nullptr);

}

std::vector<std::string> Engine::split( std::string& str, char delim){
    std::vector<std::string> result = {""};
    bool cont = false;
    for ( auto cha : str ){
        if ( cha == '"' ){
            cont = !cont;
        } else if ( cha == delim && !cont){
            result.push_back("");
        } else {
            result.back().push_back(cha);
        }
    }
    if ( result.back() == "" ){
        result.erase(--result.end());
    }
    return result;
}

void Engine::saveGamestats()
{
    Statistics* stats = tampere_.giveStats();

    std::ofstream writefile;
    writefile.open(FILELOC, std::ofstream::app);
    if( not writefile){
        qDebug() << "fail finding file to write";
        return;
    }
    else
    {
        writefile << playername->toStdString() << ";"
                  << stats->giveNysseKillCount() << ";"
                  << stats->givePassengerKillCount()
                  << "\n";
    }
    writefile.close();
}


