#include <QApplication>
#include <QDebug>
#include "engine.hh"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(offlinedata);
    Engine engine_;
    engine_.gameSetup(&a);
    return 0;
}
