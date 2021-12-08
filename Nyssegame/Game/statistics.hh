/**
* This file:
*
* Program author:
* Mika Pajala
* Johannes Kalliokoski
*
* Statistics class provides simple methods for colleting stats from the game.
* Inherits Interface IStatistics.
*/

#ifndef STATISTICS_HH
#define STATISTICS_HH

#include "../Course/CourseLib/interfaces/istatistics.hh"

class Statistics: public Interface::IStatistics
{
public:
    /**
     * @brief Statistics Creates private variables
     * and sets up the class.
     */
    Statistics();

    ~Statistics();

    /**
     * @brief morePassengers Notifies that the provided amount of
     * passengers have been added to the game.
     * Inherited from IStatistics.
     * @param num Int of players added.
     */
    void morePassengers(int num);

    /**
     * @brief nysseRemoved Records that one bus has been removed.
     * Inherited from IStatistics.
     */
    void nysseRemoved();

    /**
     * @brief newNysse Records that one bus has been added.
     * Inherited from IStatistics.
     */
    void newNysse();

    /**
     * @brief nysseLeft Unused method.
     *  Inherited from IStatistics.
     */
    void nysseLeft();

    /**
     * @brief passengerRemoved Records that a passenger has been removed.
     */
    void passengerRemoved();

    /**
     * @brief givePassengerKillCount Returns total amount of passengers removed.
     * @return int passengersremoved_
     */
    int givePassengerKillCount();

    /**
     * @brief giveNysseKillCount Return total amount of nysses removed.
     * @return int bussesremoved_
     */
    int giveNysseKillCount();

private:
    int passengersadded_;
    int passengersremoved_;
    int nyssecount_;
    int bussesremoved_;
};


#endif // STATISTICS_HH
