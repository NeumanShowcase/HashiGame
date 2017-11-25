#include "gridobject.h"

GridObject::GridObject(int x, int y):_xVectorPos(x),_yVectorPos(y)
{
    setAcceptHoverEvents(true);
}

void GridObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    emit hoverEnter(this);
}

void GridObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    emit hoverLeft(this);
}

int GridObject::getXPos()
{
    return this->_xVectorPos;
}

int GridObject::getYPos()
{
    return this->_yVectorPos;
}

GridObject::~GridObject()
{
    delete this->_itemGroup;
}

void GridObject::setPotentialLine(bool value)
{
    this->_isPotentialLine = value;
}

//Check if a line can be placed on this GridObject
bool GridObject::isPotentialLine()
{
    return this->_isPotentialLine;
}

QPoint GridObject::getPoint()
{
    return this->_pos;
}
