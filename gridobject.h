#pragma once

#include <QGraphicsItem>
#include <QObject>
#include <QBrush>
#include <QPen>
#include <QHoverEvent>
#include <QDebug>
#include <QGraphicsItemGroup>

#define WINDOWSIZE 600

class GridObject : public QObject, public QGraphicsRectItem
{

    Q_OBJECT

public:

    GridObject(int x, int y);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    int getXPos();

    int getYPos();

    void setPotentialLine(bool value);

    bool isPotentialLine();

    QPoint getPoint();

    virtual ~GridObject();

signals:

    void hoverEnter(GridObject*);

    void hoverLeft(GridObject*);

protected:

    QPoint _pos;

    bool _alreadyClicked = false;

    QGraphicsItemGroup *_itemGroup;

    int _xVectorPos ;

    int _yVectorPos;

    bool _isPotentialLine = false;

};
