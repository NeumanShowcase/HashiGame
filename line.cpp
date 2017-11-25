#include "line.h"

Line::Line(int x, int y, Orientation orientation, QPoint point, int size, Node* conn1, Node* conn2): GridObject(x, y)
{
    this->_firstConnection = conn1;
    this->_secondConnection = conn2;
    this->_pos = point;
    this->_orientation = orientation;
    this->_gridSize = size;

    //create shapes
    this->setRect(this->_pos.x(), this->_pos.y(), size, size);
    QBrush brush(Qt::transparent);
    this->setBrush(brush);
    QPen pen(Qt::transparent);
    this->setPen(pen);
    this->_itemGroup = new QGraphicsItemGroup(this);
    QBrush brush2(QColor(88,88,88));
    QPen pen2(QColor(88,88,88));

    //set orientation
    switch(this->_orientation)
    {
    case Orientation::horizontal:

        this->_lineRect = new QGraphicsRectItem(this->_pos.x() - (this->_gridSize/5), this->_pos.y() + (this->_gridSize/2), this->_gridSize * 1.4, this->_gridSize/15);
        break;

    case Orientation::vertical:

        this->_lineRect = new QGraphicsRectItem(this->_pos.x() + (this->_gridSize/2.1), this->_pos.y() - (this->_gridSize/5), this->_gridSize/15, this->_gridSize * 1.4);
        break;
    }

    //connect shapes in groups
    this->_itemGroup->addToGroup(this->_lineRect);
    this->_lineRect->setBrush(brush2);
    this->_lineRect->setPen(pen2);
    this->_lineRect->setAcceptHoverEvents(false);
    this->setZValue(0);
}

//Emit left or right click function to call slot in GameLogic class
void Line::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clickedLine(this);
    }
    else if (event->button() == Qt::RightButton)
    {
        emit rightClickedLine(this);
    }
}

//Get horizontal or vertical
Orientation Line::getOrientation()
{
    return this->_orientation;
}

//Return the first node this line is connected to
Node *Line::getFirstConnection()
{
    return this->_firstConnection;
}

//Return the second node this line is connected to
Node *Line::getSecondConnection()
{
    return this->_secondConnection;
}

bool Line::checkIsDouble()
{
    return this->_isDouble;
}

void Line::addSecondLine()
{
    this->_isDouble = true;
    this->_itemGroup->removeFromGroup(this->_lineRect);
    delete _lineRect;

    QBrush brush(QColor(88,88,88));
    QPen pen(QColor(88,88,88));

    switch(this->_orientation)
    {
        case Orientation::horizontal:
            this->_lineRect = new QGraphicsRectItem(this->_pos.x() - (this->_gridSize/5), this->_pos.y() + (this->_gridSize/2.5), this->_gridSize * 1.4, this->_gridSize/15);
            this->_secondLineRect = new QGraphicsRectItem(this->_pos.x() - (this->_gridSize/5), this->_pos.y() + (this->_gridSize/1.66), this->_gridSize * 1.4, this->_gridSize/15);
            break;

        case Orientation::vertical:
            this->_lineRect = new QGraphicsRectItem(this->_pos.x() + (this->_gridSize/2.5), this->_pos.y() - (this->_gridSize/5), this->_gridSize/15, this->_gridSize * 1.4);
            this->_secondLineRect = new QGraphicsRectItem(this->_pos.x() + (this->_gridSize/1.66), this->_pos.y() - (this->_gridSize/5), this->_gridSize/15, this->_gridSize * 1.4);
            break;
    }
    this->_itemGroup->addToGroup(_lineRect);
    this->_lineRect->setBrush(brush);
    this->_lineRect->setPen(pen);
    this->_itemGroup->addToGroup(_secondLineRect);
    this->_secondLineRect->setBrush(brush);
    this->_secondLineRect->setPen(pen);
    this->_lineRect->setAcceptHoverEvents(false);
    this->_secondLineRect->setAcceptHoverEvents(false);
}

void Line::removeSecondLine()
{
    this->_isDouble = false;
    this->_itemGroup->removeFromGroup(this->_lineRect);
    this->_itemGroup->removeFromGroup(this->_secondLineRect);
    if(this->_secondLineRect != nullptr)
    {
        delete _secondLineRect;
    }
    if(this->_lineRect != nullptr)
    {
        delete _lineRect;
    }

    QBrush brush2(QColor(88,88,88));
    QPen pen2(QColor(88,88,88));

    switch(this->_orientation)
    {
        case Orientation::horizontal:
            this->_lineRect = new QGraphicsRectItem(this->_pos.x() - (this->_gridSize/5), this->_pos.y() + (this->_gridSize/2), this->_gridSize * 1.4, this->_gridSize/15);
            break;
        case Orientation::vertical:
            this->_lineRect = new QGraphicsRectItem(this->_pos.x() + (this->_gridSize/2.1), this->_pos.y() - (this->_gridSize/5), this->_gridSize/15, this->_gridSize * 1.4);
            break;
    }

    this->_itemGroup->addToGroup(_lineRect);
    this->_lineRect->setBrush(brush2);
    this->_lineRect->setPen(pen2);
    this->_lineRect->setAcceptHoverEvents(false);
}

Line::~Line()
{
    delete _lineRect;
}

