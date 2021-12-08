#include "mainwindow.hh"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->PauseBtn->setGeometry(0, 0, 50, 25);
    ui->PauseBtn->setDisabled(true);
    ui->StartBtn->setGeometry(500, 0, 100, 25);
    ui->healthBar->setVisible(false);    
    ui->Bullet_amount->setVisible(false);
    ui->_bullet->setVisible(false);
    ui->healthBar->setStyleSheet("QProgressBar {"
                                 "background-color: #FFFFFF;"
                                 "color: #FFFFFF;"
                                 "border-color: #000000;"
                                 "border-radius: 10px;"
                                 "text-align: center; }"
                                 "QProgressBar::chunk {"
                                 "background-color: #AA0000;}");

    scene_ = new QGraphicsScene(this);
    view_ = new QGraphicsView(this);

    image_ = QImage(":/offlinedata/offlinedata/kartta_iso_1095x592.png");

    scene_->setSceneRect(0, 0, 1095*3, 592*3);

    scene_->setBackgroundBrush(image_.scaledToWidth(1095*3));

    QCursor c = QCursor(Qt::CrossCursor);
    setCursor(c);

    view_->setGeometry(53, 53, 1095, 592);
    view_->setScene(scene_);
    view_->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    view_->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

    view_->viewport()->installEventFilter(this);

    this->setMouseTracking(true);
    this->activateWindow();

    resetMovement();

    connect(&movetimer_, SIGNAL(timeout()),
            this, SLOT(checkPlayerMove()));

    movetimer_.setInterval(MOVETIME);

}

MainWindow::~MainWindow()
{

    delete ui;
    delete scene_;
    delete view_;

}
bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == view_->viewport() && event->type() == QEvent::Wheel) {
        return true;
    }
    return false;
}

void MainWindow::on_StartBtn_clicked()
{
    ui->StartBtn->setDisabled(true);
    ui->PauseBtn->setDisabled(false);
    ui->Bullet_amount->setVisible(true);
    ui->_bullet->setVisible(true);
    emit startGame();
    ui->healthBar->setVisible(true);
    movetimer_.start();
}

void MainWindow::on_PauseBtn_clicked()
{

    movetimer_.stop();
    ui->PauseBtn->setDisabled(true);
    PauseMenu p;
    p.exec();

    if (p.result() == 1){
        ui->PauseBtn->setDisabled(false);
        movetimer_.start();

    }
    else if(p.result() == 0){
        //Game closed
        resetMovement();
        hide();
        ui->StartBtn->setDisabled(false);
        ui->healthBar->setVisible(false);
        ui->Bullet_amount->setVisible(false);
        ui->_bullet->setVisible(false);
        emit restart();
    }
}

void MainWindow::checkPlayerMove()
{
    std::vector<bool> movedir = {false, false, false, false};

    if(pmoveup){
        movedir.at(UP-1) = true;
    }
    if(pmoveleft){
        movedir.at(LEFT-1) = true;
    }
    if(pmovedown){
        movedir.at(DOWN-1) = true;
    }
    if(pmoveright){
        movedir.at(RIGHT-1) = true;
    }
    emit playerMoved(movedir);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        emit shoot(view_->mapToScene(event->pos()).toPoint());
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {

    case Qt::Key_W:
        pmoveup = true;
        break;
    case Qt::Key_A:
        pmoveleft = true;
        break;
    case Qt::Key_D:
        pmoveright = true;
        break;
    case Qt::Key_S:
        pmovedown = true;
        break;

    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{

    switch (event->key()) {

    case Qt::Key_W:
        pmoveup = false;
        break;
    case Qt::Key_A:
        pmoveleft = false;
        break;
    case Qt::Key_D:
        pmoveright = false;
        break;
    case Qt::Key_S:
        pmovedown = false;
        break;
    }
}

void MainWindow::resetMovement()
{
    pmoveup = false;
    pmoveleft = false;
    pmovedown = false;
    pmoveright = false;
}


void MainWindow::setHP(int amount)
{
    ui->healthBar->setValue(amount);
}

bool MainWindow::hasGameEnded(Statistics* gamestats)
{
    movetimer_.stop();
    resetMovement();

    ui->StartBtn->setDisabled(false);
    ui->healthBar->setVisible(false);
    ui->PauseBtn->setDisabled(false);
    ui->Bullet_amount->setVisible(false);
    ui->_bullet->setVisible(false);

    ui->PauseBtn->setDisabled(true);
    QSound::play("../../codecookingkitchen/noices/au_2.wav");

    GameOverMenu d_gameover_(gamestats);
    MainWindow::hide();
    d_gameover_.exec();


    if (d_gameover_.result()) {

        //Game restarted
        emit restart();
        return false;
    }
    else {
        // exit
        //qDebug() << "gameover exit!";
        return true;
    }
}

void MainWindow::setHealthMax(int max)
{
    ui->healthBar->setMaximum(max);
    ui->healthBar->setValue(max);
    ui->Bullet_amount->display(10);
}

void MainWindow::shooted(int i)
{
    ui->Bullet_amount->display(i);
}
