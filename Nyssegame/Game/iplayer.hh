#ifndef IPLAYER_HH
#define IPLAYER_HH
#include "interfaces/iactor.hh"

class IPlayer: public virtual Interface::IActor
{
public:
    IPlayer() = default;
    virtual ~IPlayer() = default;
};

#endif // IPLAYER_HH
