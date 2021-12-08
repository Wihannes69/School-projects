#include "statistics.hh"
#include <QDebug>

Statistics::Statistics():
    Interface::IStatistics()
{

    passengersadded_ = 0;
    passengersremoved_ = 0;
    nyssecount_ = 0;
    bussesremoved_ = 0;
}

Statistics::~Statistics()
{

}

void Statistics::morePassengers(int num)
{
    passengersadded_ += num;
}

void Statistics::nysseRemoved()
{
    nyssecount_--;
    bussesremoved_++;
}

void Statistics::newNysse()
{
    nyssecount_++;
}

void Statistics::nysseLeft()
{

}

void Statistics::passengerRemoved()
{
    passengersremoved_++;
}

int Statistics::givePassengerKillCount()
{
    return passengersremoved_;
}

int Statistics::giveNysseKillCount()
{
    return bussesremoved_;
}
