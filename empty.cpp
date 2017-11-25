#include "empty.h"

Empty::Empty(int x, int y, QPoint point, int size): GridObject(x, y)
{
    this->_pos = point;
    this->setRect(_pos.x(), _pos.y(), size, size);
    QBrush brush(Qt::transparent);
    this->setBrush(brush);
    QPen pen(Qt::transparent);
    this->setPen(pen);
    this->_itemGroup = new QGraphicsItemGroup(this);
    this->setZValue(0);
}

//Calls a slot in GameLogic class if user clicks on an empty grid
void Empty::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clickedEmpty(this);
    }
}

Empty::~Empty()
{

}
