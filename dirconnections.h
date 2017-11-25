#pragma once

#include "nodeconnect.h"
#include <map>
#include <vector>
#include <gridobject.h>

enum class Direction
{
    none = 0,top,right,bottom,left
};

class DirConections
{

public:

    static Direction getOppositeDirection(Direction inDir);

    DirConections(int maxConnections);

    NodeConnect *selectConnection(Direction direction);

    std::map<Direction,int> getSlotMap();

    int getRemaining();

    void setConnections(int);

    int getConnections();

private:

    const int _maximumAmountOfBridges;

    int _currentAmountOfBridges = 0;

    std::map<Direction,NodeConnect> _DirConections;
};

