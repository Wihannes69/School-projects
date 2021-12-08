/* Nysse: Parameters
 * ----------
 * Program author
 * Name: Johannes Kalliokoski
 * Name: Mika Pajala
 *
 * Provides parameters for nearly all classes.
 * */
#include "interfaces/icity.hh"

#ifndef PARAMETERS_H
#define PARAMETERS_H

// Topscore file
const std::string FILELOC = "../../codecookingkitchen/topscore.txt";

// timer intervals
static const int BULLETTIME = 100;
static const int DAMAGETIME = 500;
static const int MOVETIME = 100;

static const double HEALTHTRESHOLD = 0.3;

// difficulty parameters
const Interface::Location EASYLOCATION = Interface::Location(6825876, 3328208);
const Interface::Location MEDIUMLOCATION = Interface::Location(6825679, 3327348);
const Interface::Location HARDLOCATION = Interface::Location(6825288, 3327661);

const int EASYHEALTH = 50;
const int MEDIUMHEALTH = 25;
const int HARDHEALTH = 5;

const int EASY = 1;
const int MEDIUM = 2;
const int HARD = 3;

// location parameters for coordinate translation
static const int EAST = 3329260; // x = 1095
static const int WEST = 3325990; // x = 0
static const int NORTH = 6826300; // y = 0
static const int SOUTH = 6824550; // y = 592
static const int WIDTH = 1095*3;
static const int HEIGHT = 592*3;

// distance for actor to be near another
static const int NEARBYDISTANCE = 25;

// limits for player movement in the scene
static const int UPLIMIT = 545;
static const int LEFTLIMIT = -336;
static const int RIGHTLIMIT = 724;
static const int DOWNLIMIT = -15;

// directions
static const int UP = 1;
static const int LEFT = 2;
static const int RIGHT = 3;
static const int DOWN = 4;

// actor shorthands
static const int NYSSE = 1;
static const int PASSENGER = 2;
static const int PLAYER = 3;

// actor sizes
static const int BUS_SIZE = 50;
static const int PASS_SIZE = 20;
static const int COVID_SIZE = 80;
static const int PLAYER_SIZE = 30;



#endif // PARAMETERS_H
