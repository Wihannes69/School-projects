/**
* This file:
*
* Program author:
* Mika Pajala
* Johannes Kalliokoski
*
* Statisticstest class provides unit tests for Statitics class in the game.
*
*/

#include <QtTest>
#include "statistics.hh"


class statisticstest : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief statisticstest Creates a new statisticsclass object
     * for running tests.
     */
    statisticstest();
    ~statisticstest();

private slots:

    void initTestCase();

    void passengertest();
    void bussesTest();
    void mixTest();

private:


};

statisticstest::statisticstest()
{
}

statisticstest::~statisticstest()
{

}

void statisticstest::initTestCase()
{
    Statistics stats_ = {};
    stats_.passengerRemoved();
    QVERIFY(stats_.givePassengerKillCount() != 0);
}

void statisticstest::passengertest()
{
    Statistics stats_ = {};
    stats_.passengerRemoved();
    qDebug() << "test";
    QCOMPARE(stats_.givePassengerKillCount(), 1);
}

void statisticstest::bussesTest(){
    Statistics stats_;
    stats_.nysseRemoved();
    QVERIFY(stats_.giveNysseKillCount() == 1);
}

void statisticstest::mixTest()
{
    Statistics stats_;
    for(int i=0;i<10;i++){
        stats_.nysseRemoved();
        stats_.passengerRemoved();
    }
    QCOMPARE(stats_.giveNysseKillCount(), stats_.givePassengerKillCount());
    QVERIFY(stats_.giveNysseKillCount() == 10);

}

QTEST_MAIN(statisticstest)
#include "statisticstest.moc"
